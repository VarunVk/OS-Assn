import pytest
import os
import subprocess

def test_compile():
    os.system("make clean")
    os.system("make")
    assert("votecounter\n" == subprocess.check_output(['ls','votecounter']))

def test_basic():
    assert (True)
