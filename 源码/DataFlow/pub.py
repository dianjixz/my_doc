from dataflow import dataflow, flowtask

class task(flowtask):
    def __init__(self, conf):
        pass
    def on_data(self, data):
        print(data)
        self.out(data)



class DataFlow(dataflow):

    def rpc_setup(self, work_id, conf):
        self.creat_task('pub', task(conf), work_id)

    def rpc_exit(self, work_id):
        self.del_task(work_id)

if __name__ == '__main__':
    flow = DataFlow()
    flow.loop()