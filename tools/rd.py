import sys,struct
d=open('orig/tsh.exe','rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]
n=struct.unpack_from('<H',d,pe+6)[0]
opt=pe+24+struct.unpack_from('<H',d,pe+20)[0]
secs=[]
for i in range(n):
    name,vs,va,rs,ro=struct.unpack_from('<8sIIII',d,opt+i*40)
    secs.append((va+0x400000,vs,ro,rs))
def rd(v,n):
    for s,vs,ro,rs in secs:
        if s<=v<s+vs:
            o=v-s
            if o>=rs: return b'\0'*n
            return d[ro+o:ro+o+n]
def cstr(v):
    b=rd(v,300); return b.split(b'\0')[0].decode('cp932','replace')
if __name__=='__main__':
    mode=sys.argv[1]; a=int(sys.argv[2],16); c=int(sys.argv[3]) if len(sys.argv)>3 else 16
    if mode=='d':
        print(' '.join('%x'%x for x in struct.unpack('<%dI'%c,rd(a,4*c))))
    elif mode=='s': print(repr(cstr(a)))
    elif mode=='p':
        for x in struct.unpack('<%dI'%c,rd(a,4*c)): print('%x'%x, repr(cstr(x)))
    elif mode=='h': print(rd(a,c).hex())
