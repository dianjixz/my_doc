# conan 使用

conan install --requires=libhv/1.3.4 --requires=nng/1.9.0 --generator=SConsDeps -o libhv/*:shared=True --build=libhv/1.3.4

conan install --requires=libhv/1.3.4 --generator=SConsDeps

conan create .