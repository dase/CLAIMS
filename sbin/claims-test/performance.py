#!/usr/bin/python
import os,sys
import glob

SQLNAME = [ "tpc_sql_1", "tpc_sql_3", "tpc_sql_5", "tpc_sql_6", "tpc_sql_10", "tpc_sql_12", "tpc_sql_13", "tpc_sql_17", ]

def listresult():
    with open('./testresult/output.csv', 'wt') as handle:
        for sql in SQLNAME:
            print sql
            handle.writelines(sql + ',\n')
            filelst = glob.glob('./testresult/'+sql+'-*')
            for resfile in filelst:
                fobj = open(resfile, 'r')
                line = fobj.readline()
                while line:
                    if not line.find('tuples') == -1:
                        print line
                        handle.writelines(','+line)
                    line = fobj.readline()
                fobj.close()

if __name__=="__main__": 
    listresult()
