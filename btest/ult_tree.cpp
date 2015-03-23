
#include "stdafx.h"

#pragma comment (lib,"winmm.lib")





template<class _T>
void makeData(_T &v,int &seed)
{
    v = krand(seed);
    v ^= krand(seed)<<12;
    v ^= krand(seed)<<24;
}

void makeData(float &v,int &seed)
{
    *(int*)&v = krand(seed);
    *(int*)&v ^= krand(seed) << 12;
    *(int*)&v ^= krand(seed) << 24;
}

void makeData(vector<char> &v,int &seed)
{
    v.resize(32);
    int l = krand(seed)%v.size();
    int i = 0;
    while(i<l) v[i++] = krand(seed)%255 + 1;
    v[i++] = 0;
    while(i<(int)v.size()) v[i++] = krand(seed);
}

void makeData(string &v,int &seed)
{
    int lb = krand(seed)%5;
    int l = krand(seed)%(1<<lb);
    v.clear();
    for(int i=0;i<l;i++)
        v.push_back(krand(seed)%255+1);
}

void makeData(vector<byte> &v,int &seed)
{
    int lb = krand(seed)%5;
    v.resize(krand(seed)%(1<<lb));
    for(int i=0;i<(int)v.size();i++)
        v[i] = krand(seed);
}

void serData(TreeFileRef t,const char *name,int id,byte &v,const byte &def)     { t.SerByte(name,v,def,id); }
void serData(TreeFileRef t,const char *name,int id,word &v,const word &def)     { t.SerWord(name,v,def,id); }
void serData(TreeFileRef t,const char *name,int id,dword &v,const dword &def)   { t.SerDword(name,v,def,id); }
void serData(TreeFileRef t,const char *name,int id,char &v,const char &def)     { t.SerChar(name,v,def,id); }
void serData(TreeFileRef t,const char *name,int id,short &v,const short &def)   { t.SerShort(name,v,def,id); }
void serData(TreeFileRef t,const char *name,int id,int &v,const int &def)       { t.SerInt(name,v,def,id); }
void serData(TreeFileRef t,const char *name,int id,float &v,const float &def)   { t.SerFloat(name,v,def,id); }
void serData(TreeFileRef t,const char *name,int id,vector<char> &v,const vector<char> &def)   { t.SerPChar(name,&v[0],v.size(),&def[0],id); }
void serData(TreeFileRef t,const char *name,int id,string &v,const string &def)   { t.SerString(name,v,def.c_str(),id); }
void serData(TreeFileRef t,const char *name,int id,vector<byte> &v,const vector<byte> &def)
{
    void *vptr = v.size() ? &v[0] : NULL;
    if(t.IsWriting())
        t.Write_SetRaw(name,vptr,v.size(),id);
    else
    {
        int size = t.Read_GetRawSize(name,id);
        v.resize(size);
        vptr = v.size() ? &v[0] : NULL;
        memcpy(vptr,t.Read_GetRawData(name,id),v.size());
    }
}


template<class _T>
bool cmpData(const _T &a,const _T &b)
{
    return a == b;
}

bool cmpData(const float &a,const float &b)
{
    if( !(a==a) || !(b==b) )
        return (a==a) == (b==b);
    return a == b;
}

template<class _T>
bool cmpData(const vector<_T> &a,const vector<_T> &b)
{
    if(a.size()!=b.size())
        return false;
    for(int i=0;i<(int)a.size();i++)
        if(a[i]!=b[i])
            return false;
    return true;
}

bool cmpData(const vector<char> &a,const vector<char> &b)
{
    const char *sa=&a[0];
    const char *sb=&b[0];
    int i = 0;
    for(;sa[i];i++)
        if(sa[i]!=sb[i])
            break;
    return !sa[i] && !sb[i];
}

template<class _T>
const char *nameOf(const _T &v)             { return typeid(v).name(); }
const char *nameOf(const byte &v)           { return "byte"; }
const char *nameOf(const word &v)           { return "word"; }
const char *nameOf(const dword &v)          { return "dword"; }
const char *nameOf(const vector<char> &v)   { return "pchar"; }
const char *nameOf(const string &v)         { return "string"; }
const char *nameOf(const vector<byte> &v)   { return "raw"; }



#define M_MAKE_VALUE        (1<<0)
#define M_MAKE_DEF          (1<<1)
#define M_SEQUENCE          (1<<2)


template<class _T>
void SerBasic(TreeFileRef t,int mode,int &seed,int &junkseed,int &sid,_T *type)
{
    _T ref;
    _T value;
    _T def;

    makeData(ref,seed);

    if(mode & M_MAKE_VALUE) value = ref;
    else                    makeData(value,junkseed);

    if(mode & M_MAKE_DEF  ) def = ref;
    else                    makeData(def,junkseed);

    string name;
    int id;
    name = nameOf(value);
    if(mode&M_SEQUENCE)
        name += "_seq";
    else
    {
        int nl = krand(seed)%4+5;
        name += "_";
        for(int i=0;i<nl;i++)
            name.push_back(krand(seed)%23+'a');
    }
    if(krand(seed)%100<70 && (mode&M_SEQUENCE))
        id = ++sid;
    else
    {
        makeData(id,seed);
        sid = id;
    }
    serData(t,name.c_str(),id,value,def);

    //printf("%c %s[%d] %d %d %d %d\n",t.IsReading()?'R':'W',name.c_str(),id,seed,value,ref,def);
    assert( cmpData(value,ref) );
}

void SerRandomTree(TreeFileRef t,int mode,int &seed,int &junkseed,int size)
{
    int sid = krand(seed);
    SerBasic(t,mode,seed,junkseed,sid,(byte*)0);
    SerBasic(t,mode,seed,junkseed,sid,(word*)0);
    SerBasic(t,mode,seed,junkseed,sid,(dword*)0);
    SerBasic(t,mode,seed,junkseed,sid,(char*)0);
    SerBasic(t,mode,seed,junkseed,sid,(short*)0);
    SerBasic(t,mode,seed,junkseed,sid,(int*)0);
    SerBasic(t,mode,seed,junkseed,sid,(float*)0);
    SerBasic(t,mode,seed,junkseed,sid,(vector<char>*)0);
    SerBasic(t,mode,seed,junkseed,sid,(string*)0);
    SerBasic(t,mode,seed,junkseed,sid,(vector<byte>*)0);

    for(int i=0;i<10;i++)
        SerBasic(t,mode | M_SEQUENCE,seed,junkseed,sid,(int*)0);

    for(int i=0;i<size;i++)
    {
        string name;
        int id;
        int nl = krand(seed)%4+5;
        name = "sub_";
        for(int i=0;i<nl;i++)
            name.push_back(krand(seed)%23+'a');
        makeData(id,seed);
        SerRandomTree(t.SerChild(name.c_str(),id),mode,seed,junkseed,size-1);
    }

}




void ult_tree()
{
    { // generate file
        TreeFileBuilder tfb;
        int seed = 123;
        int junkseed = 5;
        SerRandomTree(tfb.GetRoot(true),M_MAKE_VALUE,seed,junkseed,5);
        tfb.SaveTreeTxt("tree.txt");
    }

    { // test file
        TreeFileBuilder tfb;
        int seed = 123;
        int junkseed = 5;
        tfb.LoadTreeTxt("tree.txt",NFS);
        SerRandomTree(tfb.GetRoot(false),0,seed,junkseed,5);
        tfb.SaveTreeTxt("tree2.txt");
    }
}

void ult_tree_project()
{
    { // generate file
        TreeFileBuilder tfb;
        FileReaderStream fs("project.dat");
        tfb.LoadTreeBin(&fs);
        tfb.SaveTreeTxt("project.txt");
        tfb.SaveTreeTxt("project0.txt");
    }

    for(int i=0;i<3;i++)
    {
        printf("%2d:",i);
        // test
        {
            TreeFileBuilder tfb;
            FileWriterStream fs("project.bin");
            printf(" txt");
            tfb.LoadTreeTxt("project.txt",NFS);
            printf("->bin");
            tfb.SaveTreeBin(&fs);
        }
        {
            TreeFileBuilder tfb;
            FileReaderStream fs("project.bin");
            printf("  bin");
            tfb.LoadTreeBin(&fs);
            printf("->txt");
            tfb.SaveTreeTxt("project.txt");
        }
        printf("\n");
    }
}


void ult_project_speed_test()
{
    DWORD t0;

    t0 = timeGetTime();
    {
        TreeFileBuilder tfb;
        tfb.LoadTreeTxt("project_txt.dat",NFS);
    }
    printf("Text load time: %dms\n",timeGetTime()-t0);

    t0 = timeGetTime();
    {
        TreeFileBuilder tfb;
        FileReaderStream fs("project_bin.dat");
        tfb.LoadTreeBin(&fs);
    }
    printf("Binary load time: %dms\n",timeGetTime()-t0);

    printf("\n\n\n");
    system("pause");
}

void ult_config()
{
	Config cfg("test.cfg");
	printf("Ala: %s\n",cfg.GetString("Ala","?").c_str());

    printf("\n\n\n");
    system("pause");
}
