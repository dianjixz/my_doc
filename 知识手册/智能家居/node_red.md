

nodered 是一个很好的底代码自动化平台。



nodered 节点介绍

```test
events:state  →用作传感器数据触发。因为接入HA后传感器的entity中只有字符串形式的单个值，可以以它作为触发器使用。

current state  →用于中途判断某传感器的当前状态。状态为HA中获取的该实体的最后一次数据。

switch  →用于流程的选择，可根据不同的输入执行不同的流程。没有output properties属性，不会对传入的值进行更改。

function  →执行函数语句，本质为JavaScript语法。可在里面对msg数据进行修改并传输至下一个节点。

delay  →用于延时触发。

mqtt out  →用于发布mqtt消息至mqtt服务器

mqtt in  →用于接收mqtt服务器的消息，一般作为触发器使用。

call service  →用于触发HA中的服务，比如我们之前设置的遥控器就可以在这里进行设置自动化触发。

debug  →一般用于调试，不用自己写function来看数据。

inject  →用于作为触发节点，可设置循环型触发。

time range →用于时间段筛选，以时间段分割出一天。时间段内为in，时间段外为off

LS filter →用于替换time range节点的，通过node-red-contrib-light-scheduler安装。可以一周为单位进行计划控制。

wait until →用于阻塞整个流程进行，直到wait until条件成立时才继续执行（里面的timeout我没弄懂，我本来以为是超过timeout就强制返回一个值并进行流程，实际上我只要设置了timeout，wait until条件就不会被触发了）

```