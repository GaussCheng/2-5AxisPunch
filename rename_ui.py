import os
import sys
import re

pattern = re.compile("_[1-9].h")

if len(sys.argv) != 2:
    print "usage python rename_ui.py dir"
    exit(0)

print sys.argv[1]
target_dir = sys.argv[1]
if not os.path.isdir(target_dir):
    print "please input a dir!"
    exit(0)

import shutil
for root , dirs , files in os.walk(target_dir, True):
    for name in files:
        if pattern.match(name[-4:]):
            absname =  os.path.abspath(os.path.join(target_dir, name))
            print absname
           # print absname.replace("_8.h", ".h");
            shutil.copy(absname, absname.replace(name[-4:],".h"))
