import json
import os
from ConfigParser import RawConfigParser
import io
import re

def normalizeRegex(r):
    r = r.replace('IDENTS', r'(?:[a-zA-Z_][a-zA-Z0-9_]*(?:\s*,\s*[a-zA-Z_][a-zA-Z0-9_]*)*)')
    r = r.replace('IDENT', r'(?:[a-zA-Z_][a-zA-Z0-9_]*)')
    
    r = r.replace('{{', '(?P<names>')
    r = r.replace('}}', ')')
    
    r = r.replace(' +', r'[\x20\t]+')
    r = r.replace(' ?', r'[\x20\t]?')
    r = r.replace(' *', r'[\x20\t]*')
    r = r.replace(' ',  r'[\x20\t]+')
    
    if not r.startswith('^'):
        r = r'^\s*' + r
    return r
    
root = os.path.split(os.path.abspath( __file__ ))[0]
os.chdir(root)

definitions = {}
for p in os.listdir('.'):
    if os.path.splitext(p)[1] != '.ini':
        continue
    
    j = {
        "exts": [],
        "symbols": [],
    }
    with open(p, 'r') as f:
        inicontents = f.read()
        rcp = RawConfigParser()
        rcp.readfp(io.BytesIO(inicontents))
        
        sections = [s for s in rcp.sections() if s != 'language']
        
        j["exts"] = rcp.get("language", "exts").strip().split()
        for section in sections:
            scopes = rcp.get(section, "scope").strip().split(', ') if rcp.has_option(section, "scope") else []
            j["symbols"].append({
                "kind": section.partition("&")[0],
                "regex": rcp.get(section, "regex"),
                "scope": scopes,
            })
            try:
                if not re.compile(rcp.get(section, "regex")):
                    raise Exception("Bad regex")
            except Exception as e:
                print '%s/%s Regex /%s/ is invalid: %s' % (os.path.splitext(p)[0], section, rcp.get(section, "regex"), e)
    
    for symbol in j["symbols"]:
        if "regex" in symbol and symbol["regex"]:
            symbol["regex"] = normalizeRegex(symbol["regex"])
    
    definitions[os.path.splitext(p)[0]] = j

minified = False
with open('re2tags.json', 'w') as f:
    if minified:
        f.write(json.dumps(definitions, separators=(',',':')))
    else:
        f.write(json.dumps(definitions, sort_keys=True, indent=4))
