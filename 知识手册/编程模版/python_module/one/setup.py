from setuptools import setup,Extension
 
MOD = 'hello'
setup(name=MOD, ext_modules=[Extension(MOD, sources=['hellomodule.c'])])

