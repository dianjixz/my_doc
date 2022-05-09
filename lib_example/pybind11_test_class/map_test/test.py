# import nihao as stl
# import numpy as np


# # C++ std::vector
# # python list
# var1 = stl.vec_add([1, 3, 5, 7, 9, 11], [9, 7, 5, 4, 3, 1])
# print(var1)

# var2 = stl.vec_sin(np.linspace(0, np.pi*2, num=20).tolist())
# print(var2)

# # C++ std::map
# # python dict
# var3 = stl.get_map(['apple', 'banana', 'melon', 'orange'], [1, 2, 3, 4])
# print(type(var3))
# print(var3)

# # C++ std::set
# # python set
# var4 = stl.get_set(['a', 'd', 'f', 'asd', 'a', 'A', 'BB', 'b'])
# print(type(var4))
# print(var4)


from PIL import Image
import nihao

mk = nihao.img_data


img = Image.open("./nihao.png")


img_da = img.tobytes()

mdd = mk.send_to_c_image(img_da,img.width,img.height)
# import numpy as np

# mk = np.array(img)
# print(mk[0][0])
# print(mk[0][1])
# print(nihao.print_bytes(img_da))


















