# python 模块



python3 的模块具有多样性，它可以是一个 python 文件，一个文件夹，或者一个 so.
我们主要介绍一个文件夹的 python 模块。
```python
# 从文件夹内导入单个对象
from .util_mixins import NiceRepr

# 从文件夹内导入多个对象
from .box_transform import bbox2distance, distance2bbox

# 从文件夹内导入多个对象
from .check_point import (
    convert_avg_params,
    convert_old_model,
    load_model_weight,
    save_model,
)
# 将对象导出外部
__all__ = [
    "NiceRepr",
    "bbox2distance",
    "distance2bbox",
    "convert_avg_params",
    "convert_old_model",
    "load_model_weight",
    "save_model"
]

```