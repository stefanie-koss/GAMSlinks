using System;
using System.Runtime.InteropServices;

public class gdxcs  // C#  procedure wrapper generated by generated by apiwrapper
{


public delegate void tdatastoreproc(IntPtr indx,IntPtr vals);

[DllImport("gdxdclib",EntryPoint="cgdxsetloadpath")]'
private static extern dll_gdxsetloadpath (string s);
[DllImport("gdxdclib",EntryPoint="cgdxgetloadpath")]'
private static extern dll_gdxgetloadpath (ref byte s);

[DllImport("gdxdclib",EntryPoint="gdxacronymcount")]
private static extern int dll_gdxacronymcount(int pgdx);
[DllImport("gdxdclib",EntryPoint="cgdxacronymgetinfo")]
private static extern int dll_gdxacronymgetinfo(int pgdx,int n,ref byte aname,ref byte atext,ref int indx);
[DllImport("gdxdclib",EntryPoint="gdxacronymindex")]
private static extern int dll_gdxacronymindex(int pgdx,double v);
[DllImport("gdxdclib",EntryPoint="cgdxacronymname")]
private static extern int dll_gdxacronymname(int pgdx,double v,ref byte aname);
[DllImport("gdxdclib",EntryPoint="cgdxacronymsetinfo")]
private static extern int dll_gdxacronymsetinfo(int pgdx,int n,string aname,string atext,int indx);
[DllImport("gdxdclib",EntryPoint="gdxacronymvalue")]
private static extern double dll_gdxacronymvalue(int pgdx,int indx);
[DllImport("gdxdclib",EntryPoint="cgdxaddalias")]
private static extern int dll_gdxaddalias(int pgdx,string aname1,string aname2);
[DllImport("gdxdclib",EntryPoint="cgdxaddsettext")]
private static extern int dll_gdxaddsettext(int pgdx,string s,ref int n);
[DllImport("gdxdclib",EntryPoint="gdxautoconvert")]
private static extern int dll_gdxautoconvert(int pgdx,int nv);
[DllImport("gdxdclib",EntryPoint="gdxclose")]
private static extern int dll_gdxclose(int pgdx);
[DllImport("gdxdclib",EntryPoint="gdxdataerrorcount")]
private static extern int dll_gdxdataerrorcount(int pgdx);
[DllImport("gdxdclib",EntryPoint="gdxdataerrorrecord")]
private static extern int dll_gdxdataerrorrecord(int pgdx,int rn,ref int aelements,ref double avals);
[DllImport("gdxdclib",EntryPoint="gdxdatareaddone")]
private static extern int dll_gdxdatareaddone(int pgdx);
[DllImport("gdxdclib",EntryPoint="gdxdatareadfilteredstart")]
private static extern int dll_gdxdatareadfilteredstart(int pgdx,int synr,ref int adomainnrs,ref int nrrecs);
[DllImport("gdxdclib",EntryPoint="gdxdatareadmap")]
private static extern int dll_gdxdatareadmap(int pgdx,int rn,ref int aelements,ref double avals,ref int afdim);
[DllImport("gdxdclib",EntryPoint="gdxdatareadmapstart")]
private static extern int dll_gdxdatareadmapstart(int pgdx,int synr,ref int nrrecs);
[DllImport("gdxdclib",EntryPoint="gdxdatareadraw")]
private static extern int dll_gdxdatareadraw(int pgdx,ref int aelements,ref double avals,ref int afdim);
[DllImport("gdxdclib",EntryPoint="gdxdatareadrawstart")]
private static extern int dll_gdxdatareadrawstart(int pgdx,int synr,ref int nrrecs);
[DllImport("gdxdclib",EntryPoint="cgdxdatareadslice")]
private static extern int dll_gdxdatareadslice(int pgdx,string[] afiltelements,ref int adim,tdatastoreproc dp);
[DllImport("gdxdclib",EntryPoint="gdxdatareadslicestart")]
private static extern int dll_gdxdatareadslicestart(int pgdx,int asynr,ref int anrelems);
[DllImport("gdxdclib",EntryPoint="bgdxdatareadstr")]
private static extern int dll_gdxdatareadstr(int pgdx,ref byte astrelements,ref double avals,ref int afdim);
[DllImport("gdxdclib",EntryPoint="gdxdatareadstrstart")]
private static extern int dll_gdxdatareadstrstart(int pgdx,int synr,ref int nrrecs);
[DllImport("gdxdclib",EntryPoint="bgdxdatasliceuels")]
private static extern int dll_gdxdatasliceuels(int pgdx,ref int aindx,ref byte auels);
[DllImport("gdxdclib",EntryPoint="gdxdatawritedone")]
private static extern int dll_gdxdatawritedone(int pgdx);
[DllImport("gdxdclib",EntryPoint="gdxdatawritemap")]
private static extern int dll_gdxdatawritemap(int pgdx,ref int aelements,ref double avals);
[DllImport("gdxdclib",EntryPoint="cgdxdatawritemapstart")]
private static extern int dll_gdxdatawritemapstart(int pgdx,string aname,string atext,int adim,int atype,int auserinfo);
[DllImport("gdxdclib",EntryPoint="gdxdatawriteraw")]
private static extern int dll_gdxdatawriteraw(int pgdx,ref int aelements,ref double avals);
[DllImport("gdxdclib",EntryPoint="cgdxdatawriterawstart")]
private static extern int dll_gdxdatawriterawstart(int pgdx,string aname,string atext,int adim,int atype,int auserinfo);
[DllImport("gdxdclib",EntryPoint="cgdxdatawritestr")]
private static extern int dll_gdxdatawritestr(int pgdx,string[] astrelements,ref double avals);
[DllImport("gdxdclib",EntryPoint="cgdxdatawritestrstart")]
private static extern int dll_gdxdatawritestrstart(int pgdx,string aname,string atext,int adim,int atype,int auserinfo);
[DllImport("gdxdclib",EntryPoint="cgdxgetdllversion")]
private static extern int dll_gdxgetdllversion(int pgdx,ref byte v);
[DllImport("gdxdclib",EntryPoint="gdxerrorcount")]
private static extern int dll_gdxerrorcount(int pgdx);
[DllImport("gdxdclib",EntryPoint="cgdxerrorstr")]
private static extern int dll_gdxerrorstr(int pgdx,int n,ref byte s);
[DllImport("gdxdclib",EntryPoint="gdxfileinfo")]
private static extern int dll_gdxfileinfo(int pgdx,ref int filever,ref int comprlev);
[DllImport("gdxdclib",EntryPoint="cgdxfileversion")]
private static extern int dll_gdxfileversion(int pgdx,ref byte filestr,ref byte producestr);
[DllImport("gdxdclib",EntryPoint="gdxfilterexists")]
private static extern int dll_gdxfilterexists(int pgdx,int n);
[DllImport("gdxdclib",EntryPoint="gdxfilterregister")]
private static extern int dll_gdxfilterregister(int pgdx,int v);
[DllImport("gdxdclib",EntryPoint="gdxfilterregisterdone")]
private static extern int dll_gdxfilterregisterdone(int pgdx);
[DllImport("gdxdclib",EntryPoint="gdxfilterregisterstart")]
private static extern int dll_gdxfilterregisterstart(int pgdx,int nr);
[DllImport("gdxdclib",EntryPoint="cgdxfindsymbol")]
private static extern int dll_gdxfindsymbol(int pgdx,string aname,ref int aix);
[DllImport("gdxdclib",EntryPoint="cgdxgetelemtext")]
private static extern int dll_gdxgetelemtext(int pgdx,int n,ref byte s,ref int node);
[DllImport("gdxdclib",EntryPoint="gdxgetlasterror")]
private static extern int dll_gdxgetlasterror(int pgdx);
[DllImport("gdxdclib",EntryPoint="gdxgetspecialvalues")]
private static extern int dll_gdxgetspecialvalues(int pgdx,ref double avals);
[DllImport("gdxdclib",EntryPoint="cgdxgetuel")]
private static extern int dll_gdxgetuel(int pgdx,int uelnr,ref byte s);
[DllImport("gdxdclib",EntryPoint="gdxmapvalue")]
private static extern int dll_gdxmapvalue(int pgdx,double d,ref int sv);
[DllImport("gdxdclib",EntryPoint="cgdxopenread")]
private static extern int dll_gdxopenread(int pgdx,string afn,ref int errnr);
[DllImport("gdxdclib",EntryPoint="cgdxopenwrite")]
private static extern int dll_gdxopenwrite(int pgdx,string afn,string aproducer,ref int errnr);
[DllImport("gdxdclib",EntryPoint="cgdxopenwriteex")]
private static extern int dll_gdxopenwriteex(int pgdx,string afn,string aproducer,int compr,ref int errnr);
[DllImport("gdxdclib",EntryPoint="gdxresetspecialvalues")]
private static extern int dll_gdxresetspecialvalues(int pgdx);
[DllImport("gdxdclib",EntryPoint="gdxsethastext")]
private static extern int dll_gdxsethastext(int pgdx,int n);
[DllImport("gdxdclib",EntryPoint="gdxsetreadspecialvalues")]
private static extern int dll_gdxsetreadspecialvalues(int pgdx,ref double avals);
[DllImport("gdxdclib",EntryPoint="gdxsetspecialvalues")]
private static extern int dll_gdxsetspecialvalues(int pgdx,ref double avals);
[DllImport("gdxdclib",EntryPoint="gdxsettextnodenr")]
private static extern int dll_gdxsettextnodenr(int pgdx,int n,int node);
[DllImport("gdxdclib",EntryPoint="cgdxsettracelevel")]
private static extern int dll_gdxsettracelevel(int pgdx,int n,string s);
[DllImport("gdxdclib",EntryPoint="gdxsymbindxmaxlength")]
private static extern int dll_gdxsymbindxmaxlength(int pgdx,int synr,ref int diminfo);
[DllImport("gdxdclib",EntryPoint="gdxsymbmaxlength")]
private static extern int dll_gdxsymbmaxlength(int pgdx);
[DllImport("gdxdclib",EntryPoint="cgdxsymboladdcomment")]
private static extern int dll_gdxsymboladdcomment(int pgdx,int synr,string s);
[DllImport("gdxdclib",EntryPoint="cgdxsymbolgetcomment")]
private static extern int dll_gdxsymbolgetcomment(int pgdx,int synr,int n,ref byte s);
[DllImport("gdxdclib",EntryPoint="gdxsymbolgetdomain")]
private static extern int dll_gdxsymbolgetdomain(int pgdx,int synr,ref int domainids);
[DllImport("gdxdclib",EntryPoint="cgdxsymbolinfo")]
private static extern int dll_gdxsymbolinfo(int pgdx,int synr,ref byte aname,ref int adim,ref int atyp);
[DllImport("gdxdclib",EntryPoint="cgdxsymbolinfox")]
private static extern int dll_gdxsymbolinfox(int pgdx,int synr,ref int acount,ref int auserinfo,ref byte aexpltxt);
[DllImport("gdxdclib",EntryPoint="cgdxsymbolsetdomain")]
private static extern int dll_gdxsymbolsetdomain(int pgdx,string[] domainids);
[DllImport("gdxdclib",EntryPoint="gdxsysteminfo")]
private static extern int dll_gdxsysteminfo(int pgdx,ref int nrsy,ref int nruel);
[DllImport("gdxdclib",EntryPoint="gdxuelmaxlength")]
private static extern int dll_gdxuelmaxlength(int pgdx);
[DllImport("gdxdclib",EntryPoint="gdxuelregisterdone")]
private static extern int dll_gdxuelregisterdone(int pgdx);
[DllImport("gdxdclib",EntryPoint="cgdxuelregistermap")]
private static extern int dll_gdxuelregistermap(int pgdx,int uelnr,string elem);
[DllImport("gdxdclib",EntryPoint="gdxuelregistermapstart")]
private static extern int dll_gdxuelregistermapstart(int pgdx);
[DllImport("gdxdclib",EntryPoint="cgdxuelregisterraw")]
private static extern int dll_gdxuelregisterraw(int pgdx,string elem);
[DllImport("gdxdclib",EntryPoint="gdxuelregisterrawstart")]
private static extern int dll_gdxuelregisterrawstart(int pgdx);
[DllImport("gdxdclib",EntryPoint="cgdxuelregisterstr")]
private static extern int dll_gdxuelregisterstr(int pgdx,string elem,ref int uelnr);
[DllImport("gdxdclib",EntryPoint="gdxuelregisterstrstart")]
private static extern int dll_gdxuelregisterstrstart(int pgdx);
[DllImport("gdxdclib",EntryPoint="cgdxumfinduel")]
private static extern int dll_gdxumfinduel(int pgdx,string s,ref int en,ref int umap);
[DllImport("gdxdclib",EntryPoint="cgdxumuelget")]
private static extern int dll_gdxumuelget(int pgdx,int n,ref byte s,ref int umap);
[DllImport("gdxdclib",EntryPoint="gdxumuelinfo")]
private static extern int dll_gdxumuelinfo(int pgdx,ref int nrelem,ref int highmap);
[DllImport("gdxdclib",EntryPoint="gdxcurrentdim")]
private static extern int dll_gdxcurrentdim(int pgdx);


[DllImport("gdxdclib")]
private static extern void xcreate(ref int pgdx);

[DllImport("gdxdclib")]
private static extern void xfree(ref int pgdx);

public int gdxCreate(ref int pgdx,string msgBuf)
{
    xcreate(ref pgdx);
    if(pgdx != 0) return 1;
    return 0;
}

public int gdxCreateX(ref int pgdx,string msgBuf)
{
    xcreate(ref pgdx);
    if(pgdx != 0) return 1;
    return 0;
}

public int gdxCreateD(ref int pgdx,string dirName,string msgBuf)
{
    xcreate(ref pgdx);
    if(pgdx != 0) return 1;
    return 0;
}

public int gdxCreateL(ref int pgdx,string libName,string msgBuf)
{
    xcreate(ref pgdx);
    if(pgdx != 0) return 1;
    return 0;
}

public int gdxFree(ref int pgdx)
{
    xfree(ref pgdx);
    return 1;
}

private void ConvertC2CS(byte[] b,ref string s)
{
    int i;
    s = "";
    i = 0;
    while (b[i] != 0)
    {
        s = s + (char)(b[i]);
        i = i + 1;
    }
}

private void ConvertArrayC2CS(byte[,] b,ref string s,int k)
{
    int i;
    s = "";
    i = 0;
    while (b[k, i] != 0)
    {
        s = s + (char)(b[k, i]);
        i = i + 1;
    }
}

public gdxsetloadpath(string s);
{
   dll_gdxsetloadpath(s);
}

public gdxgetloadpath(ref string s);
{
   byte[] cpy_s = new byte[gamsglobals.str_len];
   dll_gdxgetloadpath(ref cpy_s[0]);
   ConvertC2CS(cpy_s,ref s);
}

public int gdxacronymcount(int pgdx)
{
   return dll_gdxacronymcount(pgdx);
}

public int gdxacronymgetinfo(int pgdx,int n,ref string aname,ref string atext,ref int indx)
{
   int    rc_gdxacronymgetinfo;
   byte[] cpy_aname = new byte[gamsglobals.str_len];
   byte[] cpy_atext = new byte[gamsglobals.str_len];
   rc_gdxacronymgetinfo = dll_gdxacronymgetinfo(pgdx,n,ref cpy_aname[0],ref cpy_atext[0],ref indx);
   ConvertC2CS(cpy_aname,ref aname);
   return rc_gdxacronymgetinfo;
   ConvertC2CS(cpy_atext,ref atext);
   return rc_gdxacronymgetinfo;
}

public int gdxacronymindex(int pgdx,double v)
{
   return dll_gdxacronymindex(pgdx,v);
}

public int gdxacronymname(int pgdx,double v,ref string aname)
{
   int    rc_gdxacronymname;
   byte[] cpy_aname = new byte[gamsglobals.str_len];
   rc_gdxacronymname = dll_gdxacronymname(pgdx,v,ref cpy_aname[0]);
   ConvertC2CS(cpy_aname,ref aname);
   return rc_gdxacronymname;
}

public int gdxacronymsetinfo(int pgdx,int n,string aname,string atext,int indx)
{
   return dll_gdxacronymsetinfo(pgdx,n,aname,atext,indx);
}

public double gdxacronymvalue(int pgdx,int indx)
{
   return dll_gdxacronymvalue(pgdx,indx);
}

public int gdxaddalias(int pgdx,string aname1,string aname2)
{
   return dll_gdxaddalias(pgdx,aname1,aname2);
}

public int gdxaddsettext(int pgdx,string s,ref int n)
{
   return dll_gdxaddsettext(pgdx,s,ref n);
}

public int gdxautoconvert(int pgdx,int nv)
{
   return dll_gdxautoconvert(pgdx,nv);
}

public int gdxclose(int pgdx)
{
   return dll_gdxclose(pgdx);
}

public int gdxdataerrorcount(int pgdx)
{
   return dll_gdxdataerrorcount(pgdx);
}

public int gdxdataerrorrecord(int pgdx,int rn,ref int[] aelements,ref double[] avals)
{
   return dll_gdxdataerrorrecord(pgdx,rn,ref aelements[0],ref avals[0]);
}

public int gdxdatareaddone(int pgdx)
{
   return dll_gdxdatareaddone(pgdx);
}

public int gdxdatareadfilteredstart(int pgdx,int synr,ref int[] adomainnrs,ref int nrrecs)
{
   return dll_gdxdatareadfilteredstart(pgdx,synr,ref adomainnrs[0],ref nrrecs);
}

public int gdxdatareadmap(int pgdx,int rn,ref int[] aelements,ref double[] avals,ref int afdim)
{
   return dll_gdxdatareadmap(pgdx,rn,ref aelements[0],ref avals[0],ref afdim);
}

public int gdxdatareadmapstart(int pgdx,int synr,ref int nrrecs)
{
   return dll_gdxdatareadmapstart(pgdx,synr,ref nrrecs);
}

public int gdxdatareadraw(int pgdx,ref int[] aelements,ref double[] avals,ref int afdim)
{
   return dll_gdxdatareadraw(pgdx,ref aelements[0],ref avals[0],ref afdim);
}

public int gdxdatareadrawstart(int pgdx,int synr,ref int nrrecs)
{
   return dll_gdxdatareadrawstart(pgdx,synr,ref nrrecs);
}

public int gdxdatareadslice(int pgdx,string[] afiltelements,ref int adim,tdatastoreproc dp)
{
   return dll_gdxdatareadslice(pgdx,afiltelements,ref adim,dp);
}

public int gdxdatareadslicestart(int pgdx,int asynr,ref int[] anrelems)
{
   return dll_gdxdatareadslicestart(pgdx,asynr,ref anrelems[0]);
}

public int gdxdatareadstr(int pgdx,ref string[] astrelements,ref double[] avals,ref int afdim)
{
   int    rc_gdxdatareadstr;
   byte[,] cpy_astrelements = new byte[gamsglobals.maxdim,gamsglobals.str_len];
   int i_astrelements;
   int sidim_astrelements;
   rc_gdxdatareadstr = dll_gdxdatareadstr(pgdx,ref cpy_astrelements[0,0],ref avals[0],ref afdim);
   sidim_astrelements = gdxcurrentdim(pgdx);
   for (i_astrelements = 0; i_astrelements < sidim_astrelements; i_astrelements++)
      ConvertArrayC2CS(cpy_astrelements,ref astrelements[i_astrelements], i_astrelements);
   return rc_gdxdatareadstr;
}

public int gdxdatareadstrstart(int pgdx,int synr,ref int nrrecs)
{
   return dll_gdxdatareadstrstart(pgdx,synr,ref nrrecs);
}

public int gdxdatasliceuels(int pgdx,ref int[] aindx,ref string[] auels)
{
   int    rc_gdxdatasliceuels;
   byte[,] cpy_auels = new byte[gamsglobals.maxdim,gamsglobals.str_len];
   int i_auels;
   int sidim_auels;
   rc_gdxdatasliceuels = dll_gdxdatasliceuels(pgdx,ref aindx[0],ref cpy_auels[0,0]);
   sidim_auels = gdxcurrentdim(pgdx);
   for (i_auels = 0; i_auels < sidim_auels; i_auels++)
      ConvertArrayC2CS(cpy_auels,ref auels[i_auels], i_auels);
   return rc_gdxdatasliceuels;
}

public int gdxdatawritedone(int pgdx)
{
   return dll_gdxdatawritedone(pgdx);
}

public int gdxdatawritemap(int pgdx,ref int[] aelements,ref double[] avals)
{
   return dll_gdxdatawritemap(pgdx,ref aelements[0],ref avals[0]);
}

public int gdxdatawritemapstart(int pgdx,string aname,string atext,int adim,int atype,int auserinfo)
{
   return dll_gdxdatawritemapstart(pgdx,aname,atext,adim,atype,auserinfo);
}

public int gdxdatawriteraw(int pgdx,ref int[] aelements,ref double[] avals)
{
   return dll_gdxdatawriteraw(pgdx,ref aelements[0],ref avals[0]);
}

public int gdxdatawriterawstart(int pgdx,string aname,string atext,int adim,int atype,int auserinfo)
{
   return dll_gdxdatawriterawstart(pgdx,aname,atext,adim,atype,auserinfo);
}

public int gdxdatawritestr(int pgdx,string[] astrelements,ref double[] avals)
{
   return dll_gdxdatawritestr(pgdx,astrelements,ref avals[0]);
}

public int gdxdatawritestrstart(int pgdx,string aname,string atext,int adim,int atype,int auserinfo)
{
   return dll_gdxdatawritestrstart(pgdx,aname,atext,adim,atype,auserinfo);
}

public int gdxgetdllversion(int pgdx,ref string v)
{
   int    rc_gdxgetdllversion;
   byte[] cpy_v = new byte[gamsglobals.str_len];
   rc_gdxgetdllversion = dll_gdxgetdllversion(pgdx,ref cpy_v[0]);
   ConvertC2CS(cpy_v,ref v);
   return rc_gdxgetdllversion;
}

public int gdxerrorcount(int pgdx)
{
   return dll_gdxerrorcount(pgdx);
}

public int gdxerrorstr(int pgdx,int n,ref string s)
{
   int    rc_gdxerrorstr;
   byte[] cpy_s = new byte[gamsglobals.str_len];
   rc_gdxerrorstr = dll_gdxerrorstr(pgdx,n,ref cpy_s[0]);
   ConvertC2CS(cpy_s,ref s);
   return rc_gdxerrorstr;
}

public int gdxfileinfo(int pgdx,ref int filever,ref int comprlev)
{
   return dll_gdxfileinfo(pgdx,ref filever,ref comprlev);
}

public int gdxfileversion(int pgdx,ref string filestr,ref string producestr)
{
   int    rc_gdxfileversion;
   byte[] cpy_filestr = new byte[gamsglobals.str_len];
   byte[] cpy_producestr = new byte[gamsglobals.str_len];
   rc_gdxfileversion = dll_gdxfileversion(pgdx,ref cpy_filestr[0],ref cpy_producestr[0]);
   ConvertC2CS(cpy_filestr,ref filestr);
   return rc_gdxfileversion;
   ConvertC2CS(cpy_producestr,ref producestr);
   return rc_gdxfileversion;
}

public int gdxfilterexists(int pgdx,int n)
{
   return dll_gdxfilterexists(pgdx,n);
}

public int gdxfilterregister(int pgdx,int v)
{
   return dll_gdxfilterregister(pgdx,v);
}

public int gdxfilterregisterdone(int pgdx)
{
   return dll_gdxfilterregisterdone(pgdx);
}

public int gdxfilterregisterstart(int pgdx,int nr)
{
   return dll_gdxfilterregisterstart(pgdx,nr);
}

public int gdxfindsymbol(int pgdx,string aname,ref int aix)
{
   return dll_gdxfindsymbol(pgdx,aname,ref aix);
}

public int gdxgetelemtext(int pgdx,int n,ref string s,ref int node)
{
   int    rc_gdxgetelemtext;
   byte[] cpy_s = new byte[gamsglobals.str_len];
   rc_gdxgetelemtext = dll_gdxgetelemtext(pgdx,n,ref cpy_s[0],ref node);
   ConvertC2CS(cpy_s,ref s);
   return rc_gdxgetelemtext;
}

public int gdxgetlasterror(int pgdx)
{
   return dll_gdxgetlasterror(pgdx);
}

public int gdxgetspecialvalues(int pgdx,ref double[] avals)
{
   return dll_gdxgetspecialvalues(pgdx,ref avals[0]);
}

public int gdxgetuel(int pgdx,int uelnr,ref string s)
{
   int    rc_gdxgetuel;
   byte[] cpy_s = new byte[gamsglobals.str_len];
   rc_gdxgetuel = dll_gdxgetuel(pgdx,uelnr,ref cpy_s[0]);
   ConvertC2CS(cpy_s,ref s);
   return rc_gdxgetuel;
}

public int gdxmapvalue(int pgdx,double d,ref int sv)
{
   return dll_gdxmapvalue(pgdx,d,ref sv);
}

public int gdxopenread(int pgdx,string afn,ref int errnr)
{
   return dll_gdxopenread(pgdx,afn,ref errnr);
}

public int gdxopenwrite(int pgdx,string afn,string aproducer,ref int errnr)
{
   return dll_gdxopenwrite(pgdx,afn,aproducer,ref errnr);
}

public int gdxopenwriteex(int pgdx,string afn,string aproducer,int compr,ref int errnr)
{
   return dll_gdxopenwriteex(pgdx,afn,aproducer,compr,ref errnr);
}

public int gdxresetspecialvalues(int pgdx)
{
   return dll_gdxresetspecialvalues(pgdx);
}

public int gdxsethastext(int pgdx,int n)
{
   return dll_gdxsethastext(pgdx,n);
}

public int gdxsetreadspecialvalues(int pgdx,ref double[] avals)
{
   return dll_gdxsetreadspecialvalues(pgdx,ref avals[0]);
}

public int gdxsetspecialvalues(int pgdx,ref double[] avals)
{
   return dll_gdxsetspecialvalues(pgdx,ref avals[0]);
}

public int gdxsettextnodenr(int pgdx,int n,int node)
{
   return dll_gdxsettextnodenr(pgdx,n,node);
}

public int gdxsettracelevel(int pgdx,int n,string s)
{
   return dll_gdxsettracelevel(pgdx,n,s);
}

public int gdxsymbindxmaxlength(int pgdx,int synr,ref int[] diminfo)
{
   return dll_gdxsymbindxmaxlength(pgdx,synr,ref diminfo[0]);
}

public int gdxsymbmaxlength(int pgdx)
{
   return dll_gdxsymbmaxlength(pgdx);
}

public int gdxsymboladdcomment(int pgdx,int synr,string s)
{
   return dll_gdxsymboladdcomment(pgdx,synr,s);
}

public int gdxsymbolgetcomment(int pgdx,int synr,int n,ref string s)
{
   int    rc_gdxsymbolgetcomment;
   byte[] cpy_s = new byte[gamsglobals.str_len];
   rc_gdxsymbolgetcomment = dll_gdxsymbolgetcomment(pgdx,synr,n,ref cpy_s[0]);
   ConvertC2CS(cpy_s,ref s);
   return rc_gdxsymbolgetcomment;
}

public int gdxsymbolgetdomain(int pgdx,int synr,ref int[] domainids)
{
   return dll_gdxsymbolgetdomain(pgdx,synr,ref domainids[0]);
}

public int gdxsymbolinfo(int pgdx,int synr,ref string aname,ref int adim,ref int atyp)
{
   int    rc_gdxsymbolinfo;
   byte[] cpy_aname = new byte[gamsglobals.str_len];
   rc_gdxsymbolinfo = dll_gdxsymbolinfo(pgdx,synr,ref cpy_aname[0],ref adim,ref atyp);
   ConvertC2CS(cpy_aname,ref aname);
   return rc_gdxsymbolinfo;
}

public int gdxsymbolinfox(int pgdx,int synr,ref int acount,ref int auserinfo,ref string aexpltxt)
{
   int    rc_gdxsymbolinfox;
   byte[] cpy_aexpltxt = new byte[gamsglobals.str_len];
   rc_gdxsymbolinfox = dll_gdxsymbolinfox(pgdx,synr,ref acount,ref auserinfo,ref cpy_aexpltxt[0]);
   ConvertC2CS(cpy_aexpltxt,ref aexpltxt);
   return rc_gdxsymbolinfox;
}

public int gdxsymbolsetdomain(int pgdx,string[] domainids)
{
   return dll_gdxsymbolsetdomain(pgdx,domainids);
}

public int gdxsysteminfo(int pgdx,ref int nrsy,ref int nruel)
{
   return dll_gdxsysteminfo(pgdx,ref nrsy,ref nruel);
}

public int gdxuelmaxlength(int pgdx)
{
   return dll_gdxuelmaxlength(pgdx);
}

public int gdxuelregisterdone(int pgdx)
{
   return dll_gdxuelregisterdone(pgdx);
}

public int gdxuelregistermap(int pgdx,int uelnr,string elem)
{
   return dll_gdxuelregistermap(pgdx,uelnr,elem);
}

public int gdxuelregistermapstart(int pgdx)
{
   return dll_gdxuelregistermapstart(pgdx);
}

public int gdxuelregisterraw(int pgdx,string elem)
{
   return dll_gdxuelregisterraw(pgdx,elem);
}

public int gdxuelregisterrawstart(int pgdx)
{
   return dll_gdxuelregisterrawstart(pgdx);
}

public int gdxuelregisterstr(int pgdx,string elem,ref int uelnr)
{
   return dll_gdxuelregisterstr(pgdx,elem,ref uelnr);
}

public int gdxuelregisterstrstart(int pgdx)
{
   return dll_gdxuelregisterstrstart(pgdx);
}

public int gdxumfinduel(int pgdx,string s,ref int en,ref int umap)
{
   return dll_gdxumfinduel(pgdx,s,ref en,ref umap);
}

public int gdxumuelget(int pgdx,int n,ref string s,ref int umap)
{
   int    rc_gdxumuelget;
   byte[] cpy_s = new byte[gamsglobals.str_len];
   rc_gdxumuelget = dll_gdxumuelget(pgdx,n,ref cpy_s[0],ref umap);
   ConvertC2CS(cpy_s,ref s);
   return rc_gdxumuelget;
}

public int gdxumuelinfo(int pgdx,ref int nrelem,ref int highmap)
{
   return dll_gdxumuelinfo(pgdx,ref nrelem,ref highmap);
}

public int gdxcurrentdim(int pgdx)
{
   return dll_gdxcurrentdim(pgdx);
}

}
