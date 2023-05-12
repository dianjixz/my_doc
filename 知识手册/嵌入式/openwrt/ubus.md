ubus是Openwrt实现进程间通信的一种总线机制,  由三部分协作完成通信过程：ubusd守护进程，ubus服务端，ubus客户端。

1. ubusd守护进程: 管理ubus服务端和客户端的注册，并作为服务端和客户端的中间人, 进行消息转发, 所有消息均封装成json格式。向ubus服务端传递ubus客户端的请求(call)，向ubus客户端传递ubus服务端的执行结果。

2. ubus服务端(ubus server object):  提供软件各种具体功能模块的实现(methods), 服务端的对象名称和具体的mothd名称向ubusd进行注册后，就可以被其他ubus客户端进行传呼(call) 。

3. ubus客户端(ubus client object):  是具体功能(method)的传呼方(caller), 其通用ubusd向某个ubus服务端呼叫请求(call)执行指定method。

4. 实际上，一个ubus object对象，即可以作为服务端提供各种方法(method)，也可以作为客户端来传呼(call)其他对象的方法。

5. 下面是具体的实现例子:

ubus_server.c:     向ubusd注册了一个名为"ering_uobj"的对象 ,  提供一个名称为"ering_method“的方法, 这个方法的实现依靠3个名称为"id","data","msg"的参数。 为了简单实现这个例子， 程序只将收到的这3个参数打印出来, 完成后向客户端传送一条确认消息。方法请求是通过回调函数来实现的，程序本身不会退出。

    /*------------------------ ubus_server.c -----------------------------
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
     
    An example for Openwrt UBUST communication.
     
    Midas Zhou
    ---------------------------------------------------------------------*/
    #include <stdio.h>
    #include <stdint.h>
    #include <libubus.h>
     
    static struct ubus_context *ctx;
    static struct ubus_request_data req_data;
    static struct blob_buf	bb;
     
    static int ering_handler( struct ubus_context *ctx, struct ubus_object *obj,
    			  struct ubus_request_data *req, const char *method,
    			  struct blob_attr *msg );
     
    /* Enum for EGI policy order */
    enum {
    	ERING_ID,
    	ERING_DATA,
    	ERING_MSG,
    	__ERING_MAX,
    };
     
    /* Ubus Policy */
    static const struct blobmsg_policy ering_policy[] =
    {
    	[ERING_ID]  = { .name="id", .type=BLOBMSG_TYPE_INT32},
    	[ERING_DATA] = { .name="data", .type=BLOBMSG_TYPE_INT32 },
    	[ERING_MSG] = { .name="msg", .type=BLOBMSG_TYPE_STRING },
    };
     
    /* Ubus Methods */
    static const struct ubus_method ering_methods[] =
    {
    	UBUS_METHOD("ering_method", ering_handler, ering_policy),
    };
     
    /* Ubus object type */
    static struct ubus_object_type ering_obj_type =
    	UBUS_OBJECT_TYPE("ering_uobj", ering_methods);
     
    /* Ubus object */
    static struct ubus_object ering_obj=
    {
    	.name = "ering.host", 	/* with APP name */
    	.type = &ering_obj_type,
    	.methods = ering_methods,
    	.n_methods = ARRAY_SIZE(ering_methods),
    	//.path= /* useless */
    };
     
     
    void main(void)
    {
    	int ret;
    	const char *ubus_socket=NULL; /* use default UNIX sock path: /var/run/ubus.sock */
     
    	/* 1. create an epoll instatnce descriptor poll_fd */
    	uloop_init();
     
    	/* 2. connect to ubusd and get ctx */
    	ctx=ubus_connect(ubus_socket);
    	if(ctx==NULL) {
    		printf("Fail to connect to ubusd!\n");
    		return;
    	}
     
    	/* 3. registger epoll events to uloop, start sock listening */
    	ubus_add_uloop(ctx);
     
    	/* 4. register a usb_object to ubusd */
    	ret=ubus_add_object(ctx, &ering_obj);
    	if(ret!=0) {
    		printf("Fail to register an object to ubus.\n");
    		goto UBUS_FAIL;
     
    	} else {
    		printf("Add '%s' to ubus @%u successfully.\n", ering_obj.name, ering_obj.id);
    	}
     
    	/* 5. uloop routine: events monitoring and callback provoking */
    	uloop_run();
     
     
    	uloop_done();
    UBUS_FAIL:
    	ubus_free(ctx);
    }
     
     
    /*---------------------------------------------
    A callback function for ubus methods handling
    ----------------------------------------------*/
    static int ering_handler( struct ubus_context *ctx, struct ubus_object *obj,
    			  struct ubus_request_data *req, const char *method,
    			  struct blob_attr *msg )
    {
    	struct blob_attr *tb[__ERING_MAX]; /* for parsed attr */
     
    	/* Parse blob_msg from the caller to request policy */
    	blobmsg_parse(ering_policy, ARRAY_SIZE(ering_policy), tb, blob_data(msg), blob_len(msg));
     
    	/* print request msg */
    	printf("Receive msg from caller: ");
    	if(tb[ERING_ID])
    	     printf("UBUS_ID=%u  ", blobmsg_get_u32(tb[ERING_ID]));
    	if(tb[ERING_DATA])
    	     printf("DATA=%u  ", blobmsg_get_u32(tb[ERING_DATA]));
    	if(tb[ERING_MSG])
    	     printf("MSG='%s' \n", blobmsg_data(tb[ERING_MSG]));
     
    	/* Do some job here according to caller's request */
    	/* send a reply msg to the caller for information */
    	blob_buf_init(&bb, 0);
    	blobmsg_add_string(&bb,"Ering reply", "Request is being proceeded!");
    	ubus_send_reply(ctx, req, bb.head);
    	/* 	-----  reply results to the caller -----
    	 * NOTE: we may put proceeding job in a timeout task, just to speed up service response.
    	 */
    	ubus_defer_request(ctx, req, &req_data);
    	ubus_complete_deferred_request(ctx, req, UBUS_STATUS_OK);
    }

ubus_client.c:       向ubusd注册了一个名为"ering_caller" 的对象, 没有注册任何method。它向"ering_uobj"对象传呼(call)了"ering_method"方法,  在接收到服务端反馈的消息后将它打印出来，然后退出程序。

    /*-------------------------  ubus_client.c  ------------------------
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.
     
    An example for Openwrt UBUS communication.
     
    Midas Zhou
    ------------------------------------------------------------------*/
    #include <stdio.h>
    #include <libubus.h>
    #include <libubox/blobmsg_json.h>
     
    static struct ubus_context *ctx;
    static struct blob_buf	bb;
     
    static void result_handler(struct ubus_request *req, int type, struct blob_attr *msg);
     
    /* ubus object assignment */
    static struct ubus_object ering_obj=
    {
    	.name = "ering_caller",
    	#if 0
    	.type = &ering_obj_type,
    	.methods = ering_methods,
    	.n_methods = ARRAY_SIZE(ering_methods),
    	.path= /* useless */
    	#endif
    };
     
     
    void main(void)
    {
    	int ret;
    	uint32_t host_id; /* ering host id */
    	const char *ubus_socket=NULL; /* use default UNIX sock path: /var/run/ubus.sock */
     
    	/* 1. create an epoll instatnce descriptor poll_fd */
    	uloop_init();
     
    	/* 2. connect to ubusd and get ctx */
    	ctx=ubus_connect(ubus_socket);
    	if(ctx==NULL) {
    		printf("Fail to connect to ubusd!\n");
    		return;
    	}
     
    	/* 3. registger epoll events to uloop, start sock listening */
    	ubus_add_uloop(ctx);
     
    	/* 4. register a usb_object to ubusd */
    	ret=ubus_add_object(ctx, &ering_obj);
    	if(ret!=0) {
    		printf("Fail to register an object to ubus.\n");
    		goto UBUS_FAIL;
     
    	} else {
    		printf("Add '%s' to ubus @%u successfully.\n",ering_obj.name, ering_obj.id);
    	}
     
    	/* 5. search a registered object with a given name */
    	if( ubus_lookup_id(ctx, "ering.host", &host_id) ) {
    		printf("ering.host is NOT found in ubus!\n");
    		goto UBUS_FAIL;
    	}
    	printf("ering.host is found in ubus @%u.\n",host_id);
     
    	/* 6. prepare request method policy and data */
    	blob_buf_init(&bb,0);
    	blobmsg_add_u32(&bb,"id", ering_obj.id); 	/* ERING_ID */
    	blobmsg_add_u32(&bb,"data", 123456); 		/* ERING_DATA */
    	blobmsg_add_string(&bb,"msg", "Hello, ERING!"); /* ERING_DATA */
     
    	/* 7. call the ubus host object */
    	ret=ubus_invoke(ctx, host_id, "ering_method", bb.head, result_handler, 0, 3000);
    	printf("Call result: %s\n", ubus_strerror(ret));
     
    	/* 8. uloop routine: events monitoring and callback provoking
    	      However, we just ignore uloop in this example.
    	 */
    	 //uloop_run();
     
    	uloop_done();
    UBUS_FAIL:
    	ubus_free(ctx);
    }
     
    /* callback function for ubus_invoke to process result from the host
     * Here we just print out the message.
     */
    static void result_handler(struct ubus_request *req, int type, struct blob_attr *msg)
    {
            char *strmsg;
     
            if(!msg)
                    return;
     
            strmsg=blobmsg_format_json_indent(msg,true, 0); /* 0 type of format */
            printf("Response from the host: %s\n", strmsg);
            free(strmsg); /* need to free strmsg */
    }

6.  本例子在openwrt  widora环境下编译通过, Makefile 如下, Makefile环境变量请根据自己实际情况修改:

    export STAGING_DIR=/home/midas-zhou/openwrt_widora/staging_dir
    COMMON_USRDIR=/home/midas-zhou/openwrt_widora/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/
    CC= $(STAGING_DIR)/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_uClibc-0.9.33.2/bin/mipsel-openwrt-linux-gcc
     
    CFLAGS  += -I$(COMMON_USRDIR)/include
    LDFLAGS += -L$(COMMON_USRDIR)/lib
    LIBS   += -lubus -lubox -lblobmsg_json -ljson_script -ljson-c
     
    all:	 ubus_server ubus_client
     
    ubus_server:   ubus_server.c
    	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) ubus_server.c -o ubus_server 
     
    ubus_client:   ubus_client.c
    	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) ubus_client.c -o ubus_client
     
    clean:
    	rm -rf *.o ubus_server ubus_client

7.  试着将ubus封装成EGI RING模块, 以方便程序调用。具体代码见  https://github.com/widora/ctest/tree/master/wegi/ering
————————————————
版权声明：本文为CSDN博主「Midas-Zhou」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/midaszhk/article/details/90633721