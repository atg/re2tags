import json
import os
from ConfigParser import RawConfigParser
import io
import re
import subprocess

root = os.path.split(os.path.abspath( __file__ ))[0]
os.chdir(root)

for p in os.listdir('.'):
    if os.path.splitext(p)[0] != 'test':
        continue
    with open(p, 'r') as f:
        contents = f.read()
        #print contents
        
        spec, _, doc = contents.partition('%%%')
        spec = spec.strip()
        
        with open(os.path.join('tmp', p), 'w') as tf:
            tf.write(doc)
        
        print os.path.join('tmp', p)
        actual = subprocess.check_output(['../xcode/build/Debug/re2tags', os.path.join('tmp', p)]).strip()
        if actual != spec:
            print 'Found\n' + actual + '\n\n\n'

