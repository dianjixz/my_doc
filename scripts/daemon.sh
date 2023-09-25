#!/bin/bash


DAEMON="python3 main.py"
PIDFILE=/tmp/VTrainingService.pid
DAEMON_LOG="/dev/null"
DAEMON_ERROR="/dev/null"
WAIT_TIME=3

daemon_process_pid_file=/tmp/daemon_process.pid



start()
{
    nohup $DAEMON > $DAEMON_LOG 2> $DAEMON_ERROR < /dev/null &
    echo $! > $PIDFILE
}
stop()
{
    [ -f $PIDFILE ] && kill `cat $PIDFILE` && rm $PIDFILE
}
restart()
{
    stop
    start
}

daemon_process()
{
    echo "while true                                                            " >> $1
    echo "do                                                                    " >> $1
    echo "  [ ! -f $PIDFILE ] && echo 0 > $PIDFILE                              " >> $1
    echo "  PID_NUM=\`cat $PIDFILE\`                                            " >> $1
    echo "  KILL_VAL=\`pgrep --ns \$PID_NUM\`                                      " >> $1
    echo "  if [ -z \"\$KILL_VAL\" ] ; then                                   " >> $1
    echo "      nohup $DAEMON > $DAEMON_LOG 2> $DAEMON_ERROR < /dev/null &      " >> $1
    echo "      echo \$! > $PIDFILE                                             " >> $1
    echo "  fi                                                                  " >> $1
    echo "  sleep $WAIT_TIME                                                    " >> $1
    echo "done                                                                  " >> $1
    nohup bash $1 &> /dev/null &
    ps aux | grep "$1" | grep -v grep | awk '{print $2}' > $daemon_process_pid_file
}


case "$1" in
  start)
  	start
	;;
  stop)
  	stop
	;;
  restart|reload)
  	restart
	;;
  daemon)
    if [ -f $daemon_process_pid_file ] ; then
        if [ -z "$(pgrep --ns `cat $daemon_process_pid_file`)" ] ; then
            temp_file=$(mktemp)
            daemon_process $temp_file
            # echo $temp_file
        else
            echo "daemon runing..."
        fi
    else
        temp_file=$(mktemp)
        daemon_process $temp_file
        # echo $temp_file
    fi

	;;
  daemon_stop)
    if [ -f $daemon_process_pid_file ] ; then
        if [ -z "$(pgrep --ns `cat $daemon_process_pid_file`)" ] ; then
            echo "daemon not run!"
        else
            kill `cat $daemon_process_pid_file`
            stop
        fi
    else
        echo "daemon not run!"
    fi

	;;
  *)
	echo "Usage: $0 {start|stop|restart|daemon|daemon_stop}"
	exit 1
esac

