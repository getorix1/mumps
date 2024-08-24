#	Copyright 2022 by Kevin C. O'kane
#	Feb 10, 2022

	zmain
+ #include <mumpsc/cgi.h>
+ static struct C {char * s; C *next;} *S=NULL,*p1, *p2=NULL;
+ int DirectRead(int, unsigned char *);
+ if (argc>=2) $SymPut("%",argv[1]);
+ else $SymPut("%","");
+ if (argc>2&&0) {
+     char t[STR_MAX];
+     for (int i=2; i<argc; i++) {
+           sprintf(t,"%%%d",i-1);
+           $SymPut(t,argv[i]); printf("%d .... %s ..... %s\n",i,t,argv[i]);
+           }
+      sprintf(t,"%d",argc-2);
+      $SymPut((char *)"%c",t);
+      $SymPut("%0",argv[1]);
+      }

	if %'="" do
      . if $f(%,"./") set %=$p(%,"./",2)
      . set %%="""^"_%_""""
	. s %="g @%%"
      . x %
	. halt
	write !,"Mumps Interpreter  ",$zd,!!
	for  do
	. write "> "

+ __rr: DirectRead(5,&tmp[0]);
+ if (tmp[0]==4 || tmp[0]==26) goto __ex; // ^d and ^z
+ if (tmp[0]==8) goto __rr; // bs
+ if (tmp[0]!=13&&tmp[0]!=27&&tmp[0]<32) goto __rr;
+ if (tmp[0]==27) {
+           unsigned char __t;
+ __x1: DirectRead(5,&tmp[1]);
+     DirectRead(5,&tmp[2]);
+     if (tmp[1]==91&&tmp[2]==65) {
+           if (p2==NULL) goto _drx;
+           strcpy((char*)tmp,p2->s);
+           printf("%s",tmp); 
+           p2=p2->next; if (p2==NULL) p2=S;
+ __rrx:    DirectRead(5,&__t);
+           if (__t==13) { putchar('\n'); goto __xx; }
+           if (__t==127 || __t==8) {
+                 if (strlen((char*)tmp)>0) tmp[strlen((char*)tmp)-1]=0;
+                 putchar(8);
+                 putchar(' ');
+                 putchar('\r'); printf("> %s",tmp);
+                 goto __rrx;
+                 }
+           if (__t==27) {  // esc seq
+                 if (p2!=NULL) {
+                       putchar('\n');
+                       putchar('>');
+                       putchar(' ');
+                       goto __x1; 
+                       } 
+                DirectRead(5,&__t);
+                DirectRead(5,&__t);
+                goto __rr;
+                 }
+           }
+     { int i=strlen((char*)tmp); tmp[i]=__t; tmp[i+1]=0; putchar(__t);}
+     goto __rrx;
+     }
+ if (tmp[0]==13) { goto _drx; } // else putchar((int)tmp[0]);
+ ungetc(tmp[0],stdin);
+ {
+     system("stty -echo raw");
+     int __i=0;
+     while (1) {
+__ir: tmp[__i]=getchar();
+     if (tmp[__i]==13) break;
+     if (tmp[__i]==127||tmp[__i]==8) { // del
+           if (__i==0) goto __ir;
+           putchar(8);
+           putchar(' ');
+           putchar(8);
+           tmp[__i]=0;
+           --__i;
+           if (__i<0)__i=0;
+           tmp[__i]=0;
+           goto __ir;
+           }
+     putchar(tmp[__i]);
+     __i++;
+     }
+     tmp[__i]=0;
+ system("stty echo -raw");
+     putchar('\n');
+ }
+ // if (fgets((char *)&tmp[0],STR_MAX,stdin)==NULL) return 0;
+ // tmp[strlen((char *) tmp)-1]=0;
+ if (strcmp((char *) tmp,"h")==0 || 
+     strcmp((char *) tmp,"H")==0 ||
+     strcmp((char *) tmp,"halt")==0 ||
+     strcmp((char *) tmp,"HALT")==0) { 
+ __ex:	Mglobal(GCLOSE,tmp0,tmp1,svPtr);  /* Close Globals */
+	puts("\n"); 
+	return 0; 
+	}
+ __xx:
+     if (S==NULL) {
+           S=(C *) malloc(sizeof(C));
+           if (S==NULL) goto __ex;
+           S->s=(char*)malloc(strlen((char*)tmp)+1);
+           strcpy(S->s,(char*)tmp);
+           S->next=NULL;
+           p2=S;
+           }
+     else {
+           p1=(C *) malloc(sizeof(C));
+           if (p1==NULL) goto __ex;
+           p1->next=S;
+           S=p1;
+           S->s=(char*)malloc(strlen((char*)tmp)+1);
+           strcpy(S->s,(char*)tmp);
+           p2=S;
+           }
+_end:      if (tmp[0]!=0) Interpret((const char *) tmp,svPtr);
+ _drx: 
+     if (tmp[0]!=0) putchar('\n');



