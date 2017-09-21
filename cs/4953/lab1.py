import os, pickle, pprint, hashlib

def md5(fname):
    hash_md5 = hashlib.md5()
    with open(fname, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash_md5.update(chunk)
    return hash_md5.hexdigest()

bOldHash = 1;
h={}
oldhash={}
bFirstRun = 0;

try:
    oldhash = pickle.load(open('data.pk1', 'rb'))
except (OSError, IOError) as e:
    bOldHash = 0

for dirname, dirnames, filenames in os.walk('.'):
    for subdirname in dirnames:
        pass
    for filename in filenames:
        h[str(os.path.join(dirname, filename))] = md5(os.path.join(dirname, filename))

        if bOldHash:
            if os.path.join(dirname, filename) in oldhash.iterkeys():
                if oldhash.get(os.path.join(dirname, filename)) == h.get(os.path.join(dirname, filename)):
                    #do nothing
                    pass
                else:
                    print(os.path.join(dirname, filename) + " has changed!")
            else:
                print(os.path.join(dirname, filename) + " has been added since last hash!")
        else:
            bFirstRun = 1

if bFirstRun:
    print "First run of File Monitor, will report changes on next run of program."

pickle.dump(h, open('data.pk1','wb'))

