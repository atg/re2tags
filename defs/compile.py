import json
import os
import subprocess

def normalizeRegex(r):
    r.replace('IDENTS', r'(?:[a-zA-Z_][a-zA-Z0-9]*(?:\s*,\s*[a-zA-Z_][a-zA-Z0-9]*)*)')
    r.replace('IDENT', r'(?:[a-zA-Z_][a-zA-Z0-9]*)')
    
    r.replace('{{', '(?P<names>')
    r.replace('}}', ')')
    
    r.replace(' +', '[ \t]+')
    r.replace(' ?', '[ \t]?')
    r.replace(' *', '[ \t]*')
    r.replace(' ', r'[ \t]+')
    
    if not r.startswith('^'):
        r = r'^\s*' + r

root = os.path.split(os.path.abspath( __file__ ))[0]
os.chdir(root)

definitions = {}
for p in os.listdir('.'):
    if os.path.splitext(p)[1] != 'selfml':
        continue
    
    jsonsource = subprocess.check_output(['_translate.rb', p])
    j = json.reads(jsonsource)
    
    for symbol in j["symbols"]:
        if "regex" in symbol and symbol["regex"]:
            symbol["regex"] = normalizeRegex(symbol["regex"])
    
    definitions[os.path.splitext(p)[0]] = j

with open('compiled.json', 'w') as f:
    f.write(json.dumps(definitions, separators=(',',':')))
