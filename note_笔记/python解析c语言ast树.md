```python


from clang.cindex import Index, TranslationUnit
from clang.cindex import Config
from clang.cindex import Index, CursorKind
Config.set_library_file('/usr/lib/llvm-11/lib/libclang.so.1')
# 创建索引
idx = Index.create()

# 解析文件，附加编译选项
tu = idx.parse(
    '/home/nihao/w2T/ax520/TimerCam520/projects/ax520TimerCAM/main/src/auto_config.c',
    args=['-Ibuild/config -Imain/include -Imain -Icommon -Icommon/utils -Istatic/include -I/home/nihao/w2T/ax520/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/02.SDK/AX520_SDK_V1.1.0.1.B010.FULL.20241108/AX520_SDK_V1.1.0.1.B010/msp@linux-3.10.108.uclibc/mdk@ax520.v1.1.b010.p2/out/ax520/include -I/home/nihao/w2T/ax520/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/02.SDK/AX520_SDK_V1.1.0.1.B010.FULL.20241108/AX520_SDK_V1.1.0.1.B010/msp@linux-3.10.108.uclibc/mdk@ax520.v1.1.b010.p2/out/ax520/include/nial -I/home/nihao/w2T/ax520/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/02.SDK/AX520_SDK_V1.1.0.1.B010.FULL.20241108/AX520_SDK_V1.1.0.1.B010/msp@linux-3.10.108.uclibc/mdk@ax520.v1.1.b010.p2/out/ax520/include/aisp_tune_inc -I/home/nihao/w2T/ax520/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/02.SDK/AX520_SDK_V1.1.0.1.B010.FULL.20241108/AX520_SDK_V1.1.0.1.B010/msp@linux-3.10.108.uclibc/mdk@ax520.v1.1.b010.p2/out/ax520/include/mkp_inc'],
    options=TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD
)

def get_all_struct(node):
    # if node.kind == CursorKind.STRUCT_DECL or node.kind == CursorKind.:
    print(node.kind, node.spelling, node.type.spelling)
    for child in node.get_children():
        get_all_struct(child)


get_all_struct(tu.cursor)

# class TreePrinter:
#     def __init__(self):
#         self.result = []

#     def print_object_tree(self, obj, level=0, visited=None):
#         """
#         递归遍历对象的属性树，并生成树形描述
#         :param obj: 要遍历的对象
#         :param level: 当前递归的层级（用于缩进）
#         :param visited: 用于记录已访问的对象，防止循环引用
#         """
#         if visited is None:
#             visited = set()

#         # 防止循环引用
#         if id(obj) in visited:
#             self.result.append("  " * level + f"<{type(obj).__name__} (循环引用)>")
#             return

#         visited.add(id(obj))

#         # 打印对象类型
#         self.result.append("  " * level + f"<{type(obj).__name__}>")

#         # 获取对象的属性
#         if hasattr(obj, "__dict__"):  # 如果对象有 __dict__ 属性
#             for attr, value in obj.__dict__.items():
#                 if isinstance(value, (int, float, str, bool, type(None))):  # 简单类型直接打印
#                     self.result.append("  " * (level + 1) + f"{attr}: {value}")
#                 else:  # 复杂类型递归打印
#                     self.result.append("  " * (level + 1) + f"{attr}:")
#                     self.print_object_tree(value, level + 2, visited)

#         elif isinstance(obj, (list, tuple, set)):  # 如果是容器类型
#             for index, item in enumerate(obj):
#                 self.result.append("  " * level + f"[{index}]:")
#                 self.print_object_tree(item, level + 1, visited)

#         elif isinstance(obj, dict):  # 如果是字典
#             for key, value in obj.items():
#                 self.result.append("  " * level + f"{key}:")
#                 self.print_object_tree(value, level + 1, visited)

#         else:  # 其他类型直接打印
#             self.result.append("  " * level + str(obj))
#         for asdcsd in dir(obj):
#             if not asdcsd.startswith("__"):
#                 try:
#                     # print(f"   {asdcsd}: {getattr(value, asdcsd)}")
#                     self.print_object_tree(getattr(value, asdcsd), level + 1, visited)
#                 except :
#                     print(f"   {asdcsd}: 无法获取值")



#     def to_text(self):
#         return "\n".join(self.result)


# printer = TreePrinter()
# printer.print_object_tree(tu.cursor)

# with open("tree_description.txt", "w", encoding="utf-8") as f:
#     f.write(printer.to_text())












# # # 打印诊断信息（错误/警告）
# # for diag in tu.diagnostics:
# #     print(f"Diagnostic: {diag.severity}: {diag.spelling}")

# # 遍历函数声明
# # for cursor in tu.cursor.get_children():
# #     print(cursor.kind.name)
#     # if cursor.kind.name == 'FUNCTION_DECL':
#         # print(f"Function: {cursor.spelling} at {cursor.location}")

# def process_field(field, prefix=""):
#     """
#     递归处理结构体字段，支持嵌套结构体的解析。
#     :param field: 当前字段节点
#     :param prefix: 字段名前缀，用于标识嵌套层级
#     """
#     if field.kind == CursorKind.FIELD_DECL:  # 普通字段
#         field_name = f"{prefix}{field.spelling}"
#         field_type = field.type.spelling
#         print(f"  {field_name}: {field_type}")
        
#         # 如果字段类型是嵌套的结构体，递归展开
#         if field.type.get_declaration().kind == CursorKind.STRUCT_DECL:
#             struct_cursor = field.type.get_declaration()
#             for nested_field in struct_cursor.get_children():
#                 process_field(nested_field, prefix=f"{field_name}.")
#     elif field.kind == CursorKind.STRUCT_DECL:  # 如果字段本身是嵌套结构体
#         for nested_field in field.get_children():
#             process_field(nested_field, prefix=f"{prefix}{field.spelling}.")

# def find_specific_struct(node, target_struct_name):
#     """
#     查找并打印指定结构体的字段，递归展开字段。
#     :param node: AST 节点
#     :param target_struct_name: 目标结构体名称
#     """
#     if node.kind == CursorKind.STRUCT_DECL and node.spelling == target_struct_name:
#         print(f"找到目标结构体: {node.spelling}")
#         print("字段:")
#         for child in node.get_children():
#             process_field(child, prefix="")  # 从顶层开始处理字段
#         print("=" * 40)
#     # 遍历子节点
#     for child in node.get_children():
#         find_specific_struct(child, target_struct_name)






# import inspect


# def print_ast(node, depth=0):
#     indent = "  " * depth
#     # print(f"{indent}Kind: {node.kind}, Spelling: {node.spelling}, Location: {node.location}, Type: {node.type.spelling}")
#     # for attr in dir(node):
#         # print(attr)
#     for child in node.get_children():
#         print('---------------------------------------------------------------------------------------------------')
#         for attribute in dir(child):
#             # 排除内置属性（以双下划线开头和结尾的）
#             if not attribute.startswith("__"):
#                 try:
#                     value = getattr(node, attribute)
#                     print(f"{attribute}: {value}")

#                     for asdcsd in dir(value):
#                         if not asdcsd.startswith("__"):
#                             try:
#                                 print(f"   {asdcsd}: {getattr(value, asdcsd)}")
#                             except :
#                                 print(f"   {asdcsd}: 无法获取值")






#                 except :
#                     print(f"{attribute}: 无法获取值")
#     # for key, value in node.__dict__.items():
#     #     print(f"{key}: {value}")

#     # print(dir(node))
    
#     # node.kind
#     # node.spelling
#     # node.location
#     # node.type.spelling





#     # node.get_children()



#     # for child in node.get_children():
#     #     print_ast(child, depth + 1)








# def find_struct_declarations(node):
#     """递归遍历 AST，查找结构体声明"""
#     if node.kind == CursorKind.STRUCT_DECL:
#         # print(f"结构体信息: {vars(node)}")
        
        
#         print(f"结构体名称: {node.spelling}")
#         print("字段:")
#         for child in node.get_children():
#             if child.kind == CursorKind.FIELD_DECL:
#                 print(f"  字段名: {child.spelling}, 类型: {child.type.spelling}")
#         print("=" * 40)
#     # 遍历子节点
#     for child in node.get_children():
#         find_struct_declarations(child)


# # print_ast(tu.cursor)

# # find_struct_declarations(tu.cursor)
# # find_specific_struct(tu.cursor, 'RTSP_GLOBAL_PARAM_S')

# # from parse import compile
# # from parse import parse
# # import tempfile
# # import os



# clang_cmd = '''
# clang \
#     -DSAMPLE_DEV0_INI_FILE_NAME=\"sample_dev.ini\" \
#     -Ibuild/config -Imain/include -Imain -Icommon -Icommon/utils -Istatic/include -I/home/nihao/w2T/ax520/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/02.SDK/AX520_SDK_V1.1.0.1.B010.FULL.20241108/AX520_SDK_V1.1.0.1.B010/msp@linux-3.10.108.uclibc/mdk@ax520.v1.1.b010.p2/out/ax520/include -I/home/nihao/w2T/ax520/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/02.SDK/AX520_SDK_V1.1.0.1.B010.FULL.20241108/AX520_SDK_V1.1.0.1.B010/msp@linux-3.10.108.uclibc/mdk@ax520.v1.1.b010.p2/out/ax520/include/nial -I/home/nihao/w2T/ax520/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/02.SDK/AX520_SDK_V1.1.0.1.B010.FULL.20241108/AX520_SDK_V1.1.0.1.B010/msp@linux-3.10.108.uclibc/mdk@ax520.v1.1.b010.p2/out/ax520/include/aisp_tune_inc -I/home/nihao/w2T/ax520/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/02.SDK/AX520_SDK_V1.1.0.1.B010.FULL.20241108/AX520_SDK_V1.1.0.1.B010/msp@linux-3.10.108.uclibc/mdk@ax520.v1.1.b010.p2/out/ax520/include/mkp_inc \
#     -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics  \
#     main/src/sample_venc.c  2>/dev/null > {}
# '''
# # struct_name = 'tagTestVencConfig'



# # temp_file = tempfile.NamedTemporaryFile(mode='w+', delete=False)
# # try:
# #     print("临时文件路径:", temp_file.name)
# #     temp_file.write("This is a temporary file.")
# #     temp_file.close()  # 需要先关闭才能操作文件


# #     os.system(clang_cmd.format(temp_file.name))
# #     struct_dic = {}
# #     status = 0
# #     now_struct_name = ''
# #     with open(temp_file.name) as f:
# #         for line in f:
# #             if status == 0:
# #                 result = parse('|-RecordDecl {} struct {} definition{}', line)
# #                 if result is not None:
# #                     # print(line, end='')
# #                     now_struct_name = result[1]
# #                     struct_dic[now_struct_name] = []
# #                     status = 1
# #             elif status == 1:
# #                 haha = line.replace('referenced', '')
# #                 result = parse('| |-FieldDecl {}', haha)
# #                 if result is not None:
# #                     bianliang_name = haha.split()[6]
# #                     bianliang_type = haha[haha.find(bianliang_name) + len(bianliang_name)+1:].replace('\'', '').replace('\n', '')
# #                     struct_dic[now_struct_name].append((bianliang_name, bianliang_type))
# #                     # print(bianliang_name, bianliang_type)
                        
# #                 else:
# #                     status = 0


# #     hahahah = []
# #     num_spaces = '{}.'.format(struct_name)
# #     def print_iteam(stru):
# #         global num_spaces
# #         for i in stru:
# #             # print(i)
# #             # print(num_spaces, end='')
# #             bianliang_name = i[0]
# #             bianliang_type = i[1]
# #             if 'struct' in bianliang_type:
# #                 bianliang_type = bianliang_type[bianliang_type.find('struct') + len('struct') + 1:]
            
# #             # print(bianliang_name, bianliang_type)
# #             if bianliang_type in struct_dic:
# #                 num_spaces += bianliang_name + '.'
# #                 print_iteam(struct_dic[bianliang_type])
# #                 num_spaces = '{}.'.format(struct_name)
# #                 # print(bianliang_name, "yes")
# #             else:
# #                 # print(bianliang_type.split(':')[0], '\t', num_spaces+bianliang_name)
# #                 hahahah.append((bianliang_type.split(':')[0], num_spaces+bianliang_name, '='))

# #     print_iteam(struct_dic[struct_name])
# #     col_widths = [max(len(item) for item in col) for col in zip(*hahahah)]
# #     print('----------------------------------------------------')
# #     for row in hahahah:
# #         print("".join(item.ljust(width + 2) for item, width in zip(row, col_widths)))



# # finally:
# #     os.unlink(temp_file.name)  # 手动删除文件















# 解析c语言结构体使用


import sys
from clang.cindex import Index, CursorKind, TypeKind, TranslationUnit

from clang.cindex import Config

Config.set_library_file("/usr/lib/llvm-11/lib/libclang.so.1")


def parse_struct(cursor):
    """
    解析结构体中的字段信息
    """
    fields = []
    for child in cursor.get_children():
        if child.kind == CursorKind.FIELD_DECL:
            field_name = child.spelling
            field_type = child.type.spelling
            fields.append((field_name, field_type))
    return fields


def print_struct_hierarchy(struct_name, fields, prefix=""):
    """
    打印结构体的层次结构
    """
    global all_structs, typedef_to_struct_map, struct_to_typedef_map
    for field_name, field_type in fields:
        # print(field_type)
        if field_type.endswith("]"):
            pos = field_type.rfind("[")
            if pos:
                field_type = field_type[: pos - 1]
        # print(f'>>{field_type}<<')
        if (
            field_type.startswith("struct ")
            or field_type.startswith("union ")
            or field_type in typedef_to_struct_map
        ):
            # 如果字段是另一个结构体类型，递归解析
            sub_struct_name = field_type.replace("struct ", "").replace("union ", "")
            sub_cursor = find_struct_by_name(sub_struct_name)
            if sub_cursor:
                print(f"{prefix}{struct_name}.{field_name}  \t\t\t\t// {field_type}")
                sub_fields = parse_struct(sub_cursor)
                print_struct_hierarchy(
                    field_name, sub_fields, prefix=f"{prefix}{struct_name}."
                )
        else:
            print(f"{prefix}{struct_name}.{field_name}  \t\t\t\t// {field_type}")


def find_struct_by_name(name):
    """
    查找指定名称的结构体定义或 typedef 名称对应的结构体
    """
    # print(f'-------:{name} ')
    if name in typedef_to_struct_map:
        name = typedef_to_struct_map[name]

    for cursor in all_structs:
        if cursor.spelling == name:
            return cursor
    return None


def find_all_structs_and_typedefs(cursor):
    """
    遍历 AST，找到所有结构体和 typedef 定义
    """
    global all_structs, typedef_to_struct_map

    if cursor.kind == CursorKind.STRUCT_DECL and cursor.is_definition():
        all_structs.append(cursor)

    if cursor.kind == CursorKind.TYPEDEF_DECL:
        # 处理 typedef 定义
        for child in cursor.get_children():
            if child.kind == CursorKind.STRUCT_DECL:
                typedef_to_struct_map[cursor.spelling] = child.spelling

    for child in cursor.get_children():
        find_all_structs_and_typedefs(child)


def find_all_structs_and_print_hierarchy(cursor):
    """
    遍历 AST，找到所有结构体，并打印层次结构
    """
    global all_structs, typedef_to_struct_map, struct_to_typedef_map
    if cursor.kind == CursorKind.STRUCT_DECL and cursor.is_definition():
        struct_name = cursor.spelling
        fields = parse_struct(cursor)
        # print(f"{struct_name}:")
        define_name = ""
        if struct_name in struct_to_typedef_map:
            define_name = struct_to_typedef_map[struct_name]
        print(f"{define_name}:{struct_name}")
        print_struct_hierarchy(struct_name, fields)
        print()

    for child in cursor.get_children():
        find_all_structs_and_print_hierarchy(child)


if __name__ == "__main__":
    # 检查是否提供了 C 文件路径

    # 初始化 libclang
    index = Index.create()
    translation_unit = index.parse(
        "/home/nihao/w2T/ax520/TimerCam520/projects/ax520TimerCAM/main/src/sample_venc.c",
        args=[
            "-Ibuild/config",
            "-Imain/include",
            "-Imain",
            "-Icommon",
            "-Icommon/utils",
            "-Istatic/include",
            "-I/home/nihao/w2T/ax520/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/02.SDK/AX520_SDK_V1.1.0.1.B010.FULL.20241108/AX520_SDK_V1.1.0.1.B010/msp@linux-3.10.108.uclibc/mdk@ax520.v1.1.b010.p2/out/ax520/include",
            "-I/home/nihao/w2T/ax520/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/02.SDK/AX520_SDK_V1.1.0.1.B010.FULL.20241108/AX520_SDK_V1.1.0.1.B010/msp@linux-3.10.108.uclibc/mdk@ax520.v1.1.b010.p2/out/ax520/include/nial",
            "-I/home/nihao/w2T/ax520/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/02.SDK/AX520_SDK_V1.1.0.1.B010.FULL.20241108/AX520_SDK_V1.1.0.1.B010/msp@linux-3.10.108.uclibc/mdk@ax520.v1.1.b010.p2/out/ax520/include/aisp_tune_inc",
            "-I/home/nihao/w2T/ax520/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/AX520C_R1.24.4501_AX520_SDK_V1.1.0.1.B010_M5Stack/02.SDK/AX520_SDK_V1.1.0.1.B010.FULL.20241108/AX520_SDK_V1.1.0.1.B010/msp@linux-3.10.108.uclibc/mdk@ax520.v1.1.b010.p2/out/ax520/include/mkp_inc",
            "-I/home/nihao/w2T/ax520/TimerCam520/SDK/github_source/cJSON",
        ],
        options=TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD,
    )

    # 获取 AST 的根节点
    root_cursor = translation_unit.cursor

    # 初始化全局存储
    all_structs = []
    typedef_to_struct_map = {}
    struct_to_typedef_map = {}

    # 遍历 AST，找到所有结构体和 typedef 定义
    find_all_structs_and_typedefs(root_cursor)
    for i in typedef_to_struct_map:
        struct_to_typedef_map[typedef_to_struct_map[i]] = i
    # print(typedef_to_struct_map)
    # for i in typedef_to_struct_map:
    #     print(i, typedef_to_struct_map[i])
    # 打印所有结构体的层次结构
    find_all_structs_and_print_hierarchy(root_cursor)












```



不怎么好用，因为有些结构体没有定义，但是有typedef