# Need to skip sdist, since include_dirs relative path wont work (ie. sdist is created in cwd then extracted to temp dir where the wheel is built)
# python -m build --wheel
# pip install --no-clean .
# python setup.py clean --all

import pybind11
from setuptools import setup, Extension

# cpp_args = ['-fopenmp']
sfc_module = Extension(
    'biosim',
    sources=[
        'src/binder.cpp',
        'lib/Agent.cpp',
        'lib/Config.cpp',
        'lib/Genome.cpp',
        'lib/NeuralNet.cpp',
        'lib/Sim.cpp'
    ],
    language='c++',
    include_dirs=[pybind11.get_include(), 'lib/include'],
    # extra_compile_args=cpp_args,
)

setup(
    name='biosim',
    version='0.0.1',
    description='Python package with superfastcode2 C++ extension (PyBind11)',
    ext_modules=[sfc_module],
)