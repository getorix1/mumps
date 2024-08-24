//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#+                                                                  
//#+    #     # #     # #     # ######   #####
//#+    ##   ## #     # ##   ## #     # #     #
//#+    # # # # #     # # # # # #     # #
//#+    #  #  # #     # #  #  # ######   #####
//#+    #     # #     # #     # #             #
//#+    #     # #     # #     # #       #     #
//#+    #     #  #####  #     # #        #####
//#+    
//#+    Mumps Interpreter                            
//#+
//#+    Copyright (C) MM  Kevin C. O'Kane
//#+
//#+    Kevin C. O'Kane
//#+    Computer Science Department
//#+    University of Northern Iowa
//#+    Cedar Falls, IA 50614-0507
//#+    Tel 319 273 7322
//#+    okane@cs.uni.edu
//#+    http://www.cs.uni.edu/~okane
//#+
//#+    This program is free software; you can redistribute it and/or modify
//#+    it under the terms of the GNU General Public License as published by
//#+    the Free Software Foundation; either version 2 of the License, or
//#+    (at your option) any later version.
//#+
//#+    This program is distributed in the hope that it will be useful,
//#+    but WITHOUT ANY WARRANTY; without even the implied warranty of
//#+    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#+    GNU General Public License for more details.
//#+
//#+    You should have received a copy of the GNU General Public License
//#+    along with this program; if not, write to the Free Software
//#+    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#+
//#+                                                              
//#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define LT -1
#define EQ 0
#define GT 1

int slteqgt(unsigned char *s1,unsigned char *s2)

/* compares two strings */

{	while (1) {
		if (*s1<*s2) return(LT);
		if (*s1>*s2) return(GT);
		if (*s1==*s2&&!*s1) return(EQ);
		s1++; s2++;
		}
	}

/* the following are locations in the conflation table */

#define FIN 1

struct  Endings {
	char *prefix; /*ending string*/
	int offset; /*length*/
	char *replace; /*replacement*/
	};

struct Endings prefix[] = {

{	"PRE",		3,	"", },
{	"POST",		4,	"", },
{	"NON",		3,	"", },
{	"",		0,	""  },
};

struct Ends {
	char *ending; /*ending string*/
	int offset; /*length*/
	char *replace; /*replacement*/
	int replen; /*length*/
	int next; /*goto*/
	}

D_endings[] = {

{	"NNED",		4,	"N",		1,	FIN, },
{	"RATED",	5,	"RATE",		4,	FIN, },
{	"CATED",	5,	"CATE",		4,	FIN, }, /* sophisticated */
{	"IATED",	5,	"IATE",		4,	FIN, }, /* associated */
{	"MATED",	5,	"MATE",		4,	FIN, }, /* animated automated */
{	"GATED",	5,	"GATE",		4,	FIN, }, /* aggregated */
{	"ATED", 	4,	"TE",		2,	FIN, },
{	"ETED", 	4,	"TE",		2,	FIN, }, /* competed */
{	"ATED", 	4,	"TE",		2,	FIN, }, /* alternated */
{	"OTED", 	4,	"TE",		2,	FIN, }, /* promoted */
{	"UTED", 	4,	"TE",		2,	FIN, }, /* distributed */
{	"STED",		4,	"STE",		3,	FIN, }, /* wasted */
{	"TED",		3,	"T",		1,	FIN, },
{	"ERED", 	4,	"ER",		2,	FIN, }, /* offered */
{	"DDED", 	4,	"DD",		2,	FIN, }, /* added */
{	"RED",		3,	"RE",		2,	FIN, },
{	"ZED",		3,	"ZE",		2,	FIN, },
{	"ODED",		4,	"DE",		2,	FIN, }, /* coded */
{	"OVED",		4,	"VE",		2,	FIN, },
{	"EVED",		4,	"EVE",		3,	FIN, },
{	"IED",		3,	"Y",		1,	FIN, },
{	"CED",		3,	"CE",		2,	FIN, },
{	"OLLED",	4,	"OL",		2,	FIN, }, /* controlled */
{	"ALLED",	4,	"AL",		2,	FIN, }, /* called */
{	"OOLED",	5,	"00L",		3,	FIN, }, /* cooled */
{	"OLED",		4,	"L",		1,	FIN, }, /* cooled */
{	"LED",		3,	"LE",		2,	FIN, }, /* scheduled */
{	"EED",		3,	"EED",		3,	FIN, }, /* speed */
{	"ASED",		4,	"ASED",		4,	FIN, }, /* based */
{	"SSED",		4,	"SS",		2,	FIN, }, /* addressed */
{	"USED",		4,	"US",		2,	FIN, }, /* focus */
{	"SED",		3,	"SE",		2,	FIN, }, /* closed */
{	"VED",		3,	"VE",		2,	FIN, }, /* archived */
{	"NED",		3,	"N",		1,	FIN, }, /* assign */
{	"ACHED",	6,	"ACHE",		4,	FIN, }, /* cached */
{	"HED",		3,	"H",		1,	FIN, }, /* attached */
{	"GED",		3,	"GE",		2,	FIN, }, /* charged */
{	"MMED",		4,	"M",		1,	FIN, }, /* programmed */
{	"UED",		3,	"UE",		2,	FIN, }, /* queued */
{	"BED",		3,	"BE",		2,	FIN, }, /* described */
{	"DED",		3,	"DE",		2,	FIN, }, /* included */
{	"ORED",		4,	"OR",		2,	FIN, }, /* author */
{	"ED",		2,	"",		0,	FIN, },
{	"",		0,	"",		0,	FIN  },
},

S_endings[] = {

{	"TIVENESS",	5,	"TIVE", 	4,	FIN, }, /* competitiveness */
{	"INESS",	5,	"INESS",	0,	FIN, },
{	"NESS",		4,	"",		0,	FIN, },
{	"SS",		2,	"SS",		1,	FIN, },
{	"SSES",		4,	"SS",		2,	FIN, },
{	"XES",		3,	"X",		1,	FIN, },
{	"XAS",		3,	"XAS",		3,	FIN, }, /* Texas */
{	"IOUS",		4,	"IOUS",		4,	FIN, },
{	"NOUS",		4,	"NOUS",		4,	FIN, }, /* asynchronous */
{	"UOUS",		4,	"E",		1,	FIN, }, /* continuous */
{	"OUS",		3,	"",		0,	FIN, },
{	"ARIES",	5,	"ARY",		3,	FIN, },
{	"ERIES",	5,	"ERY",		3,	FIN, },
{	"ORIES",	5,	"ORY",		3,	FIN, }, /* accessories */
{	"TRIES",	5,	"TRY",		4,	FIN, }, /* countries */
{	"RIES",		4,	"RIES",		4,	FIN, },
{	"IES",		3,	"Y",		1,	FIN, },
{	"BIOS",		4,	"BIOS",		4,	FIN, }, /* netbios */
{	"DUS",		3,	"DUS",		3,	FIN, }, /* Aldus */
{	"VES",		3,	"VE",		2,	FIN, },
{	"RES",		3,	"RE",	       	2,	FIN, },

{	"SIS",		3,	"SIS",		3,	FIN, },
{	"CES",		3,	"CE",		2,	FIN, },
{	"DOS",		3,	"DOS",		3,	FIN, },
{	"CUS",		3,	"CUS",		3,	FIN, }, /* focus */
{	"TUS",		3,	"TUS",		3,	FIN, }, /* Lotus */
{	"AYS",		3,	"AY",		2,	FIN, }, /* gateways */
{	"NYS",		3,	"NY",		2,	FIN, }, /* companys */
{	"YS",		2,	"YS",		2,	FIN, }, /* Unisys */
{	"IUS",		3,	"IUS",		3,	FIN, }, /* radius */
{	"IS",		2,	"",		0,	FIN, },
{	"HES",		3,	"H",		1,	FIN, }, /* approach */
{	"VES",		3,	"VE",		2,	FIN, }, /* archives */
{	"TES",		3,	"TE",		2,	FIN, }, /* competes */
{	"US",		2,	"US",		2,	FIN, }, /* virus */
{	"S",		1,	"",		0,	0,   },
{	"",		0,	"",		0,	0    },
},

G_endings[] ={

{	"ZING",		4,	"ZE",		2,	FIN, },
{	"OLLING",	5,	"L",		2,	FIN, },
{	"LLING",	5,	"LL",		2,	FIN, }, /* calling */
{	"ALING",	5,	"AL",		2,	FIN, }, /* appealing */
{	"LING",		4,	"LE",		2,	FIN, },
{	"WRITTING",	7,	"WRITE",	5,	FIN, },
{	"ITING",	5,	"IT",		2,	FIN, },
{	"IPTING",	6,	"IPT",		3,	FIN, },
{	"PTING",	5,	"T",		1,	FIN, },
{	"MMING",	5,	"M",		1,	FIN, },
{	"CTING",	5,	"CT",		2,	FIN, },
{	"INTING",	6,	"INT",		3,	FIN, },
{	"NTING",	5,	"NT",		2,	FIN, },
{	"FTING",	5,	"FT",		2,	FIN, },
{	"RTING",	5,	"RT",		2,	FIN, },
{	"ATTING",	6,	"AT",		2,	FIN, },
{	"RATING",	6,	"RATE",		4,	FIN, }, /* operating */
{	"CATING",	6,	"CATE",		4,	FIN, }, /* communicating */
{	"DATING",	6,	"DATE",		4,	FIN, }, /* accomodating */
{	"NATING",	6,	"NATE",		4,	FIN, }, /* alternating */
{	"MATING",	6,	"MATE",		4,	FIN, }, /* automating */
{	"EATING",	6,	"EATE",		4,	FIN, }, /* automating */
{	"ATING",	5,	"AT",		2,	FIN, },
{	"STING",	5,	"ST",		2,	FIN, }, /* testing */
{	"ETING",	5,	"ET",		2,	FIN, }, /* meeting */
{	"TING",		4,	"TE",		2,	FIN, },
{	"STING",	5,	"ST",		2,	FIN, },
{	"RMING",	5,	"RM",		2,	FIN, }, /* alarming */
{	"MMING",	5,	"M",		1,	FIN, }, /* programming */
{	"MING",		4,	"ME",		2,	FIN, },
{	"ERING",	5,	"ER",		2,	FIN, }, /* offering */
{	"ORING",	5,	"OR",		2,	FIN, }, /* monitoring mirroring */
{	"RING",		4,	"RE",		2,	FIN, },
{	"RNING",	5,	"RN",		2,	FIN, },
{	"NNING",	5,	"N",		1,	FIN, },
{	"ONING",	5,	"ON",		2,	FIN, },
{	"GNING",	5,	"GN",		2,	FIN, }, /* designing */
{	"INING",	5,	"N",		1,	FIN, }, /* training */
{	"NING",		4,	"NE",		2,	FIN, },
{	"CING",		4,	"CE",		2,	FIN, },
{	"VING",		4,	"VE",		2,	FIN, },
{	"SSING",	5,	"SS",		2,	FIN, }, /* processing */
{	"SING",		4,	"SE",		2,	FIN, }, /* licensing */
{	"GING",		4,	"GE",		2,	FIN, }, /* charging */
{	"BING",		4,	"BE",		2,	FIN, }, /* describing */
{	"ING",		3,	"",		0,	FIN, },
{	"",		0,	"",		0,	FIN  },
},

N_endings[] = {

{	"ATURATION",	9,	"ATURATE",	7,	FIN, },
{	"ERATION",	7,	"ERATE",	5,	FIN, }, /* operation */
{	"TRATION",	7,	"TER",		3,	FIN, }, /* administration */
{	"RATION",	6,	"RE",		2,	FIN, },
{	"TITION",	6,	"TE",		2,	FIN, }, /* competition */
{	"ECTION",	6,	"ECT",		3,	FIN, }, /* connection */
{	"ERSION",	6,	"ERT",		3,	FIN, }, /* conversion */
{	"MATION",	6,	"MATE",		4,	FIN, }, /* animation */
{	"OTATION",	7,	"OTATE",	5,	FIN, }, /* annotation */
{	"CATION",	6,	"CATE",		4,	FIN, }, /* communication */
{	"ICTION",	6,	"ICTION",	6,	FIN, }, /* jurisdiction */
{	"NCTION",	6,	"NCTION",	6,	FIN, }, /* function */
{	"CTION",	5,	"CT",		2,	FIN, }, /* abstraction */
{	"LATION",	6,	"LATE",		4,	FIN, }, /* demodulation */
{	"TATION",	6,	"T",		1,	FIN, },

{	"NICIAN",	6,	"NICAL",	5,	FIN, }, /* technician */
{	"ICAN",		4,	"ICA",		3,	FIN, }, /* American */
{	"",		0,	"",		0,	FIN  },
},

E_endings[] = {


{	"RABLE",	5,	"ARE",		3,	FIN, }, /* comparable */
{	"EABLE",	5,	"E",		1,	FIN, }, /* aggreeable */
{	"ABLE",		4,	"",		0,	FIN, },
{	"TIBLE",	5,	"TIBLE",	5,	FIN, },
{	"IBLE",		4,	"",		0,	FIN, },
/* {	"E",		1,	"E",		1,	FIN, }, ??? why is this here */

{	"TITIVE",	6,	"TE",		2,	FIN, }, /* competitive */
{	"ECTIVE",	6,	"ECT",		3,	FIN, }, /* connective */
{	"NATIVE",	6,	"NATE",		4,	FIN, }, /* alternative */

{	"LYZE",		4,	"LYSIS",	5,	FIN, }, /* analyze */
{	"",		0,	"",		0,	FIN  },
},

L_endings[] = {

{	"TIONAL",	6,	"TION",		4,	FIN, }, /* additional */
{	"TIAL",		4,	"TIAL",		4,	FIN, },
{	"IAL",		3,	"IAL",		3,	FIN, },
{	"ERAL",		4,	"ERAL",		4,	FIN, }, /* general */
{	"RAL",		3,	"RE",		2,	FIN, },
{	"UAL",		3,	"UAL",		3,	FIN, },
{	"MAL",		3,	"MAL",		3,	FIN, },
{	"ONAL",		4,	"ON",		2,	FIN, }, /* traditional */
{	"NAL",		3,	"NAL",		3,	FIN, }, /* journal */
{	"HICAL",	5,	"IC",		2,	FIN, }, /* graphical */
{	"CAL",		3,	"CAL",		3,	FIN, }, /* technical */
{	"EAL",		3,	"EAL",		3,	FIN, }, /* appeal */
{	"BAL",		3,	"BAL",		3,	FIN, }, /* global */
{	"SAL",		3,	"SAL",		3,	FIN, }, /* universal */
{	"AL",		2,	"",		0,	FIN, },
{	"",		0,	"",		0,	FIN  },
},

C_endings[] = {

{	"ISTIC",	5,	"",		0,	FIN, },
{	"IFIC",		4,	"",		0,	FIN, },
{	"THMIC",	5,	"THM",		3,	FIN, }, /* algorithmic */
{	"MATIC",	5,	"MATE",		4,	FIN, }, /* automatic */
{	"",		0,	"",		0,	FIN  },
},

Y_endings[] = {

{	"RABLY",	5,	"RE",		2,	FIN, }, /* comparably */
{	"ABLY",		4,	"",		0,	FIN, },
{	"IBLY",		4,	"",		0,	FIN, },
{	"ILY",		3,	"",		0,	FIN, },
{	"TICALLY",	7,	"TE",		2,	FIN, }, /* automatically */
{	"ICALLY",	6,	"ICAL",		4,	FIN, }, /* techincally */
{	"CALLY",	5,	"C",		1,	FIN, },
{	"TITIVELY",	6,	"TE",		2,	FIN, }, /* competitively */
{	"BLY",		3,	"BLY",		3,	FIN, }, /* assembly */
{	"LY",		2,	"",		0,	FIN, },

{	"TARY",		4,	"TARY",		4,	FIN, },
{	"SARY",		4,	"SARY", 	4,	FIN, }, /* necessary */
{	"RARY",		4,	"RARY",		4,	FIN, }, /* library */
{	"ARY",		3,	"",		0,	FIN, },

{	"ABILITY",	7,	"",		0,	FIN, },
{	"TIBILITY",	8,	"TIBLE",	5,	FIN, },
{	"IBILITY",	7,	"",		0,	FIN, },
{	"TIVITY",	6,	"T",		1,	FIN, }, /* connectivity */
{	"VITY",		4,	"VE",		2,	FIN, },
{	"SITY",		4,	"SITY",		4,	FIN, },
{	"RITY",		4,	"RE",		2,	FIN, },
{	"CILITY",  	6,	"CILITY",	6,	FIN, },
{	"ALITY",	5,	"",		0,	FIN, },
{	"NITY",		4,	"NITY",		4,	FIN, }, /* community */
{	"CITY",		4,	"CITY",		4,	FIN, }, /* capacity */
{	"ITY",  	3,	"",		0,	FIN, },
{	"IFY",		3,	"",		0,	FIN, },

{	"UITRY",	5,	"UIT",		3,	FIN, }, /* circuitry */
{	"",             0,	"",		0,	FIN  },
},


       V_endings[] = {

{	"IV",		2,	"",		0,	FIN, },
{ 	"OLV",		3,	"OLUT",		4,	FIN, },
{	"",		0,	"",		0,	FIN  },
},

T_endings[] = {

{	"EEMENT",	6,	"EE",		2,	FIN, }, /* agreement */
{	"RMENT",	5,	"RMENT",	5,	FIN, }, /* deferment */
{	"EMENT",	5,	"E",		1,	FIN, }, /* announcement */
{	"AT",		2,	"",		0,	FIN, },
{	"ANTT",		4,	"ANTT",		4,	-1,  },
{	"LYST",		4,	"LYSIS",	5,	FIN, }, /* analyst */
{	"",		0,	"",		0,	FIN  },
},

R_endings[] = {

{	"ECTOR",	5,	"ECT",		3,	FIN, }, /* connector */
{	"TITOR",	5,	"TE",		2,	FIN, }, /* competitor */
{	"RATOR",	5,	"TE",   	2,	FIN, }, /* accelerator */
{	"MATOR",	5,	"MATE",		4,	FIN, }, /* animator */
{	"SUPER",	5,	"SUPER",	5,	FIN, }, /* super */
{	"SUPPLIER",	8,	"SUPPLY",	6,	FIN, }, /* supplier */
{	"LATOR",	5,	"LATE", 	4,	FIN, }, /* demodulator */

{	"IFIER",	5,	"IFY",		3,	FIN, }, /* amplifier */

{	"LYSER",	5,	"LYZE",		4,	FIN, }, /* analyser */
{	"LYZER",	5,	"LYZE",		4,	FIN, }, /* analyzer */

{	"DER",		3,	"D",		1,	FIN, }, /* loader */
{	"ERTER",	5,	"ERT",		3,	FIN, }, /* converter */
{	"PILER",	5,	"PILE",		4,	FIN, }, /* compiler */
{	"ZER",		3,	"ZE",		2,	FIN, }, /* compiler */
{	"NNER",		4,	"N",		1,	FIN, },
{	"",		0,	"",		0,	FIN  },
};


