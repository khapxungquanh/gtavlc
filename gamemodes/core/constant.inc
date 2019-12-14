#include <a_samp>

new const Float: DMVCP[18][3] = {
	{2172.0027,2298.2273,10.3300},
	{2219.1875,2363.6394,10.3299},
	{2244.9216,2409.8489,10.3336},
	{2376.6045,2410.8696,10.3548},
	{2424.9717,2349.3726,10.3299},
	{2424.5264,2264.1650,10.3300},
	{2488.5559,2231.2961,10.3300},
	{2525.3450,2182.8171,10.3290},
	{2525.8613,2064.6467,10.3300},
	{2450.4668,2055.6296,10.3300},
	{2356.5959,2074.7148,10.3300},
	{2356.4453,2111.7051,10.3379},
	{2305.4783,2149.7112,10.3300},
	{2194.6895,2150.9063,10.3300},
	{2150.0137,2175.6887,10.3300},
	{2153.5183,2253.9102,10.3300},
	{2174.2385,2280.2588,10.6719},
	{0.0, 0.0, 0.0}
};
new const Float: DMVBOAT[15][3] = {
	{2377.1782,438.8686,-0.4943}, {2473.7009,445.2059,-0.3238}, {2622.2649,459.3369,-0.4244}, {2756.1067,446.9867,-0.4712}, 
	{2822.5505,333.1665,-0.5678}, {2897.5107,195.9577,-0.7258}, {2929.8652,64.7664,-0.2692}, {2920.9690,-97.1028,-0.7008}, 
	{2890.9424,-315.8755,-0.5231}, {2966.0452,-533.4891,-0.3418}, {2959.2424,-728.9568,-0.4131}, {2949.9434,-894.3902,-0.6329}, 
	{2945.4087,-991.4231,-0.4088}, {2956.0088,-1158.3190,-0.5338}, {0.0, 0.0, 0.0}
};

new const Float: RentSpawn[8][4] = {
	{2281.6992,-58.7333,26.5199,267.9764}, {2281.0015,-62.1649,26.2793,269.7460}, {2280.9663,-65.4004,26.2132,270.7137}, {2280.6392,-68.6786,26.2222,271.7507},
	{2281.1611,-73.0703,26.2173,270.7565}, {2280.2058,-77.5069,26.2342,267.4035}, {2280.9963,-81.7123,26.2225,271.6241}, {2280.8948,-86.3854,26.2035,268.7644}
};

new const RentModel[1][8] = { {426, 506, 560, 480, 550, 522, 462, 481} };

new const BizzName[18][] = {
	{"Banca"},
	{"Gun Shop"},
	{"Club"},
	{"Restaurant"},
	{"Sex Shop"},
	{"24/7"}, 
	{"Clothes Store"}, 
	{"Burgher Shot"},
	{"Casino"}, 
	{"CNN"},
	{"Rent Car"}, 
	{"Paint"},
	{"Gas Station"}, 
	{"Race"},
	{"PNS"}, 
	{"Pizza"},
	{"Tunning"},
	{"Fish Shop"}
};

new const GunMafie[5][] = {
	{"Deagle"}, {"M4"}, {"Rifle"}, {"TEC9"}, {"Combat shotgun"}
};
new const GunMafieID[1][5] = {
	{24, 31, 33, 32, 27}
};

new const ValueMafie[5][2] = {
	{220, 200}, {310, 320}, {535, 320}, {450, 400}, {530, 250}
};

new CrateName[5][30] = {
	{"Rare"},
	{"Epic"}, 
	{"Legendary"}, 
	{"Transcendent"}, 
	{"Vehicle"}
};

new CratePrice[][] = {
	{100, 20},
	{200, 40},
	{1000, 100},
	{2500, 250},
	{7000, 700}
};

new CrateColors[5][10] = {
	{"F09E05"}, 
	{"F09E05"},
	{"F09E05"},
	{"e52222"},
	{"7556ff"}
};
new Float: QuestPos[25][3] = { 
	{2405.5603,-1511.6052,24.0068},
	{2009.7008,-1553.6687,14.1476},
	{1717.3066,-1680.6714,20.1964},
	{1688.6017,-1974.8695,8.8256},
	{2676.2280,-1429.9240,16.2572},
	{2149.3423,91.3951,27.2296},
	{2294.1917,547.6178,1.7944},
	{1768.7968,624.5909,10.8203},
	{2057.4829,1491.0737,11.4122},
	{2178.9543,1499.6031,10.8203},
	{2033.4104,1955.5468,12.0589},
	{1987.0634,2143.1887,10.8203},
	{3155.1462,2070.7114,14.9371},
	{3282.2405,2501.1802,14.8811},
	{3093.7832,1932.8975,23.2592},
	{3500.0996,2125.9116,14.8714},
	{2884.4426,2113.5359,10.8165},
	{2814.9246,2586.3955,9.8255},
	{2581.2864,2384.8999,15.6328},
	{2471.1970,1905.4407,9.8585},
	{2330.9797,1521.5917,42.8203},
	{2509.4553,912.3052,10.8203},
	{2201.2673,673.6022,11.3594},
	{1606.4561,664.8124,10.8203},
	{1319.7727,1255.6089,14.2731}
};
new Float: JailPos[6][3] = {
	{1913.2332,-1611.3098,1770.7469},{1914.0725,-1607.0884,1770.7469},{1913.9998,-1602.8716,1770.7469},
	{1940.0131,-1602.7235,1770.7469},{1939.2504,-1606.9781,1770.7469},{1939.9885,-1611.2113,1770.7469}
};
new Float: Training[13][3] = {
	{246.4252,88.0092,1003.6406},{275.5803,188.2351,1007.1719},{275.5803,188.2351,1007.1719},{2561.1777,-1291.2654,1054.6406},{2561.1777,-1291.2654,1054.6406},
	{2561.1777,-1291.2654,1054.6406},{1495.6749,1309.5129,1093.2837},{613.4427,3.4193,1000.9219},{-2029.7333,-119.6239,1035.1719},{2561.1777,-1291.2654,1054.6406},
	{-2170.3540,635.3894,1052.3750},{1727.0537,-1637.8325,20.2229},{1495.6749,1309.5129,1093.2837}
};

new ColorFaction[16][180] = {
	{"FFFFFF"}, {"112ef2"}, {"2b45f5"}, {"01026b"}, {"0CAB3C"}, {"11F2F2"},
	{"FAD400"}, {"00FF80"}, {"FFFFA8"}, {"FFABFC"}, {"AB00FA"}, {"7E3937"},
	{"FFFF00"}, {"f86448"}, {"112ef2"}, {"32905C"}
};
new Float:InteriorPos[37][3] = {
	{223.0797, 1287.7438, 1082.1406},{2282.7927, -1139.3999, 1050.8984},{328.0025, 1478.5465, 1084.4375},{-42.5438, 1406.3199, 1084.4297},{300.8615, 309.8871, 1003.3047},
	{2308.8101, -1212.2432, 1049.0234},{ 2233.6196, -1114.5808, 1050.8828},{2259.8242, -1135.7554, 1050.6328},{2217.8169, -1076.1425, 1050.4844},{267.1584, 305.2281, 999.1484},
	{244.1047, 304.9235, 999.1484}, {-734.8098,1589.6681,1.6145}, {235.4070, 1187.3690, 1080.2578}, {490.8654, 1399.1970, 1080.2578},{24.0680, 1340.5543, 1084.3750},{-283.9743, 1471.1327, 1084.3750},
	{-261.2778, 1456.7443, 1084.3672},{83.0025, 1322.8451, 1083.8662},{2317.9451, -1026.1736, 1050.2178},{2496.0198, -1692.6827, 1014.7422},{2196.0349, -1204.3772, 1049.0234},
	{376.2806, 1417.5321, 1081.3281},{2269.6677, -1210.3624, 1047.5625},{447.3249, 1397.5758, 1084.3047},{386.5427, 1471.8826, 1080.1875},{22.8908, 1403.7777, 1084.4297},
	{2365.3372, -1134.8403, 1050.8750},{2237.6023, -1080.8840, 1049.0234},{295.0533, 1473.0192, 1080.2578},{221.9701, 1140.8445, 1082.6094},{-68.7771, 1351.9674, 1080.2109},
	{260.8823, 1237.9570, 1084.2578},{2468.4509, -1698.2510, 1013.5078},{140.1820, 1366.4713, 1083.8594},{225.6624, 1022.0758, 1084.0159},{234.2033, 1064.1516, 1084.2117},
	{226.7048, 1114.2527, 1080.9950}
};

new Float: DealerCarPos[34][4] = {
{1441.2734,-2224.8093,13.2740,179.4625},{1447.6174,-2225.2344,13.2740,180.0775},{1457.7729,-2225.1973,13.2740,178.9312},{1495.6588,-2211.5061,13.2757,178.8022},
{1505.4369,-2211.8225,13.2778,178.7448},{1512.1740,-2211.8308,13.2778,178.5274},{1518.5363,-2211.7400,13.2817,180.4461},{1529.4902,-2211.4417,13.2817,179.7449},
{1536.2568,-2211.5039,13.2817,180.0051},{1542.9261,-2211.6558,13.2817,181.0608},{1549.3169,-2211.5176,13.2817,179.4744},{1560.7589,-2237.7512,13.2744,91.2762},
{1560.3783,-2244.2268,13.2748,91.6340},{1560.4049,-2250.9280,13.2748,88.9586},{1560.2371,-2257.3584,13.2747,90.5259},{1560.6239,-2264.1594,13.2738,89.3386},
{1560.4745,-2328.5295,13.2759,89.5027},{1560.5192,-2321.9229,13.2758,91.2465},{1560.6782,-2315.3286,13.2749,89.1229},{1560.2701,-2308.4749,13.2726,90.8419},
{1560.3086,-2335.0269,13.2739,90.0393},{1552.8674,-2360.8315,13.2817,0.8747},{1546.1908,-2361.0525,13.2817,359.5896},{1539.5558,-2361.2266,13.2817,0.4531},
{1505.6609,-2361.1614,13.2778,1.7197},{1512.1021,-2361.2539,13.2778,359.3044},{1518.7278,-2361.1372,13.2805,359.3376},{1529.8168,-2361.1741,13.2817,0.7694},
{1495.6238,-2361.1960,13.2736,0.0703},{1458.0022,-2347.6514,13.2740,359.3102},{1451.4670,-2347.6658,13.2740,359.5402},{1444.7390,-2347.3774,13.2740,359.0860},
{1398.2257,-2347.8931,13.2661,359.4724},{1407.8877,-2347.3940,13.2661,0.8884}
};
new Float: DealerCarPos1[8][4] = {
{-1843.7559,1294.2961,59.9107,198.8924},{-1835.2655,1297.2742,59.8975,199.2954},{-1835.2655,1297.2742,59.8975,199.2954},{-1827.8151,1299.3569,59.9086,197.2848},
{-1816.9409,1303.0677,59.9116,190.2576},{-1816.9409,1303.0677,59.9116,190.2577},{-1806.0881,1303.3514,59.9100,181.4281},{-1794.0687,1303.6813,59.8991,173.3053}
};
new Float: DealerCarPos2[5][4] = {
{-1416.3143,-20.0038,15.0739,0.0630},{-1416.3143,-20.0038,15.0739,0.0630},{-1302.5134,84.2867,15.0662,359.9970},{-1507.8579,-106.1037,15.0845,359.9987},{-1638.9340,-239.4042,15.0781,359.9972}
};

new Float: SafePos[8][3] = {
{302.92111, -142.86078, 999.06842},{503.12457, -10.13973, 1000.07971},{-222.43982, 1400.58081, 27.18424},
{1210.80481, 5.24004, 1000.36121},{-778.80103, 497.09222, 1371.14819},{-99.10441, -23.19045, 1000.12872},
{-31.15454, -186.73994, 1002.97852},{2314.28271, -0.10613, 26.13725}
};

new Float: RobPos[4][3] = {
{184.3517, -319.0276, 1.6316},{376.3061, -1886.5225, 2.2223},{365.3175, -1328.5284, 14.5018},{2336.5144, -1247.0845, 22.5699}
};

new Float: BoatVehPos[7][4] = {
{179.0296, -1916.2565, -0.0159},{187.7530, -1916.3357, -0.0159},{202.3899, -1916.0319, -0.0159},
{209.8458, -1915.7878, -0.0159},{227.6768, -1916.4446, -0.0159},{253.8787, -1915.4661, -0.0159},
{279.1484, -1941.1598, -0.0159}
};

new Float: HeliVehPos[7][4] = {
{-1256.2849, -41.0140, 14.6912},{-1281.7654, -9.5575, 14.6912},{-1295.4089, -22.2600, 14.6912},
{-1304.4703, 24.8014, 14.6912},{-1321.0928, 9.4536, 14.6912},{-1310.6278, 62.2550, 14.6912},
{-1330.3777, 41.6995, 14.6912}
};
// rc_arena
new Float:PaintballSpawns[7][3] = {
{-973.6214,1025.1903,1345.0475},{-973.1999,1095.4812,1344.9856},{-1032.6256,1078.1367,1343.2416},{-1054.3453,1025.1804,1343.1748},
{-1088.7957,1038.0369,1343.3666},{-1131.7955,1023.3311,1345.7217},{-1132.3667,1093.1659,1345.7927}
};
// farm
new Float:PaintballSpawns2[7][3] = {
{-136.8825, -70.0581, 3.2065},{-98.5009, -31.0434, 3.2065},{-82.4544, 24.8792, 3.2957},
{43.7600, 35.2099, 2.5184},{-6.8738, 91.7253, 3.1657},{-57.5785, 117.1301, 3.2078},
{-217.7475, -25.7134, 3.3158}
};
// island
new Float:PaintballSpawns3[7][3] = {
{1394.5992, 2784.1523, 11.1936},{1317.0734, 2852.8516, 10.8923},{1280.4998, 2738.1919, 10.9092},{1147.8285, 2788.3435, 11.0812},
{1143.5356, 2751.1782, 11.2707},{1259.3728, 2786.4995, 10.9397},{1354.5094, 2738.2000, 10.8767}
};
// desert
new Float:PaintballSpawns4[7][3] = {
{-745.0526, 1580.3075, 27.0509},{-774.2742, 1530.0056, 27.1533},{-805.3869, 1436.9856, 13.9201},{-725.0034, 1542.5139, 39.4333},
{-766.1290, 1625.1650, 27.2306},{-885.7818, 1476.7595, 24.8592},{-797.1280, 1557.1274, 27.2289}
};
// beach
new Float:PaintballSpawns5[7][3] = {
{507.8840, -1887.1956, 3.3077},{571.3418, -1871.5217, 4.4363},{609.7338, -1837.2462, 5.9157},{681.5817, -1863.5522, 5.3781},
{611.6556, -1881.2391, 4.0900},{470.7314, -1821.2793, 5.6802},{585.5042, -1800.3372, 6.2130}
};

new Float:PaintballSpawns6[7][3] = {
{-527.5182,-98.1491,63.2969},{-442.8741,-59.8322,59.1411},{-465.7904,-178.1876,78.0874},{-534.7496,-177.6923,78.4047},
{-593.2002,-110.6649,67.4507},{-485.6090,-87.7349,60.8669},{-573.0599,-89.7404,64.4440}
};
stock achievementNames(id) {
	new string[64];
	switch(id) {
		case 0: string = "Skill maxim la un job";
		case 1: string = "Achizitioneaza un vehicul";
		case 2: string = "Achizitioneaza o afacere";
		case 3: string = "Achizitioneaza o casa";
		case 4: string = "Achizitioneaza o pereche de ochelari";
		case 5: string = "Achizitioneaza o palarie";
		case 6: string = "Achizitioneaza licenta de condus";
		case 7: string = "Achizitioneaza licenta de port arma";
		case 8: string = "Achizitioneaza licenta de navigatie";
		case 9: string = "Achizitioneaza licenta de zburat";
		case 10: string = "Adauga un prieten in lista";
		case 11: string = "Participa la o cursa in arena de curse";
		case 12: string = "Castiga un meci in CS arena";
		case 13: string = "Jefuieste o afacere";
		case 14: string = "Omoara un politist";
		case 15: string = "Scapa de wanted";
		case 16: string = "Evadeaza din jail";
		case 17: string = "Casatoreste-te";
		case 18: string = "Intra intr-o factiune pasnica";
		case 19: string = "Intra intr-o factiune mixta";
		case 20: string = "Intra intr-un departament";
		case 21: string = "Intra intr-o mafie";
		case 22: string = "Stai intr-o factiune pasnica mai mult de 30 de zile";
		case 23: string = "Stai intr-o factiune mixta mai mult de 30 de zile";
		case 24: string = "Stai intr-un departament mai mult de 30 de zile";
		case 25: string = "Stai intr-o mafie mai mult de 30 de zile";
		case 26: string = "Obtine functia de helper";
		case 27: string = "Obtine functia de admin";
		case 28: string = "Obtine functia de lider intr-o factiune pasnica";
		case 29: string = "Obtine functia de lider intr-o factiune mixta";
		case 30: string = "Obtine functia de lider intr-un departament";
		case 31: string = "Obtine functia de lider intr-o mafie";
	}	
	return string;
}	
// TOW CAR CHECKPOINT
new Float:TowCarCk[3][3] = {
	{1625.0532,1074.2133,10.8203},{1638.0239,2198.6133,10.8203},{1625.0532,1074.2133,10.8203}
};
new wantedList[16][2] = {
	{1, 0},
	{1, 0},
	{2, 0},
	{2, 0},
	{2, 0},
	{3, 0},
	{3, 0},
	{3, 1},
	{3, 0},
	{4, 0},
	{4, 0},
	{4, 0},
	{5, 0},
	{5, 1},
	{6, 1},
	{6, 1}
};
stock ticketName(id) {
	new string[64];
	switch(id) {
		case 0: string = "- Parcatul neregulamentar";
		case 1: string = "- Condus neregulamentar";
		case 2: string = "- Incurcarea celorlalti participanti la trafic";
		case 3: string = "- Folosirea hidraulicelor pe un drum public";
		case 4: string = "- Folosirea de NOS";
		case 5: string = "- Headlights off (Faruri oprite)";
		case 6: string = "- Vanzarea de arme";
		case 7: string = "- Mers pe partea carosabila";	
		case 8: string = "- Condus fara casca";	
	}
	return string;
}
stock ticketValue(id) {
	new x;
	switch(id) {
		case 0: x = 9500;
		case 1: x = 9500;
		case 2: x = 9000;
		case 3: x = 3600;
		case 4: x = 7200;
		case 5: x = 10000;
		case 6: x = 5400;
		case 7: x = 5400;
		case 8: x = 8000;
	}
	return x;
}
stock wantedName(id) {
	new string[64];
	switch(id) {
		case 0: string = "Furt Vehicul";
		case 1: string = "Apel Nefondat 112";
		case 2: string = "Atacare Civil";
		case 3: string = "Neconformare Ordin";
		case 4: string = "Patrundere Teren Guvernamental";
		case 5: string = "Rapire Civil";
		case 6: string = "Posesie/Folosire Droguri";
		case 7: string = "Atacare Politist";
		case 8: string = "Neplata Amenda";
		case 9: string = "Drive-By";
		case 10: string = "Mituire Politist";
		case 11: string = "Omorare Civil";
		case 12: string = "Rapire Politist";
		case 13: string = "Omorare Politist";
		case 14: string = "Fugar";
		case 15: string = "Complice";		
	}
	return string;
}
new const HudName[13][180] = {
	{"{FFFFFF}Dezactivat"},
	{"{B8BFC2}Grey"},
	{"{FF0000}Red"},
	{"{00FF2F}Green"},
	{"{0E8C19}Dark Green"},	
	{"{0022FF}Blue"},
	{"{00AAFF}Lightblue"},
	{"{00FFD0}Cyan"},	
	{"{FFFB00}Yellow"},
	{"{FF8800}Orange"},	
	{"{FFB957}Gold"},
	{"{8000FF}Purple"},
	{"{FF0077}Pink"}	
};
new RadioNames[][180] = {
	{"{FF0000}Opreste radio-ul"},
	{"Pro FM Romania"},
	{"Radio Bandit Romania"},	
	{"Radio Taraf Romania"},	
	{"Radio Hot Romania"},	
	{"Dubstep.fm"},
	{"Radio Hit Romania"},
	{"Radio ClubMix Romania"},
	{"Kiss FM Romania [Audio Plugin]"},	
	{"Radio Zu Romania [Audio Plugin]"},
	{"Radio Popular"},	
	{"Trap.FM"},
	{"Rock FM"},
	{"PirateFM Rap"},
	{"Radio Vocea Strazii"},
	{"Radio Traditional HIP HOP"},
	{"Hot 108 Jamz"},
	{"Radio Coxet"}
};
new RadioNames2[][180] = {
	{"{FF0000}Opreste radio-ul"},
	{"Pro FM Romania"},
	{"Bandit Romania"},	
	{"Taraf Romania"},	
	{"Hot Romania"},	
	{"Dubstep.fm"},
	{"Hit Romania"},
	{"ClubMix Romania"},
	{"Kiss FM Romania [Audio Plugin]"},	
	{"Zu Romania [Audio Plugin]"},
	{"Popular"},	
	{"Trap.FM"},
	{"Rock FM"},
	{"PirateFM Rap"},
	{"Vocea Strazii"},
	{"Traditional HIP HOP"},
	{"Hot 108 Jamz"},
	{"Radio Coxet"}
};
new RadioLink[][180] = {
	{""},
	{"http://stream.profm.ro:8012/profm.mp3"},
	{"http://live.radiobandit.ro:8000"},	
	{"http://radiotaraf.com/live.m3u"},	
	{"http://live.radiohot.ro:8000/"},	
	{"http://dubstep.fm/128.pls"},
	{"http://www.radio-hit.ro/asculta.m3u"},
	{"http://live.radioclubmix.ro:9999"},
	{"http://80.86.106.136/listen.pls"},	
	{"http://www.radiozu.ro/live.m3u"},
	{"http://livemp3.radiopopular.ro:7777"},	
	{"http://radio.trap.fm/listen128.pls"},	
	{"http://80.86.106.143:9128/rockfm.aacp"},
	{"http://live.piratefm.ro:1200"},
	{"http://radio.voceastrazii.net:8012"},
	{"http://traditionalhiphop.zapto.org:7500/listen.pls"},
	{"http://sc.hot108.com:4020/listen.pls"},
	{"http://37.59.207.68:8000/"}
};
new StagePrice[1][4] = {
	{0, 120, 275, 500}
};
new StageSpeed[1][4] = {
	{0, 10, 35, 85}
};
new aVehicleNames[][] = {
	"Landstalker", "Bravura", "Buffalo", "Linerunner", "Perrenial", "Sentinel",
	"Dumper", "Firetruck", "Trashmaster", "Stretch", "Manana", "Infernus",
	"Voodoo", "Pony", "Mule", "Cheetah", "Ambulance", "Leviathan", "Moonbeam",
	"Esperanto", "Taxi", "Washington", "Bobcat", "Whoopee", "BF Injection",
	"Hunter", "Premier", "Enforcer", "Securicar", "Banshee", "Predator", "Bus",
	"Rhino", "Barracks", "Hotknife", "Trailer", "Previon", "Coach", "Cabbie",
	"Stallion", "Rumpo", "RC Bandit", "Romero", "Packer", "Monster", "Admiral",
	"Squalo", "Seasparrow", "Pizzaboy", "Tram", "Trailer", "Turismo", "Speeder",
	"Reefer", "Tropic", "Flatbed", "Yankee", "Caddy", "Solair", "Berkley's RC Van",
	"Skimmer", "PCJ-600", "Faggio", "Freeway", "RC Baron", "RC Raider", "Glendale",
	"Oceanic","Sanchez", "Sparrow", "Patriot", "Quad", "Coastguard", "Dinghy",
	"Hermes", "Sabre", "Rustler", "ZR-350", "Walton", "Regina", "Comet", "BMX",
	"Burrito", "Camper", "Marquis", "Baggage", "Dozer", "Maverick", "News Chopper",
	"Rancher", "FBI Rancher", "Virgo", "Greenwood", "Jetmax", "Hotring Racer C", "Sandking",
	"Blista Compact", "Police Maverick", "Boxville", "Benson", "Mesa", "RC Goblin",
	"Hotring Racer A", "Hotring Racer B", "Bloodring Banger", "Rancher", "Super GT",
	"Elegant", "Journey", "Bike", "Mountain Bike", "Beagle", "Cropduster", "Stunt",
 	"Tanker", "Roadtrain", "Nebula", "Majestic", "Buccaneer", "Shamal", "Hydra",
 	"FCR-900", "NRG-500", "HPV1000", "Cement Truck", "Tow Truck", "Fortune",
 	"Cadrona", "FBI Truck", "Willard", "Forklift", "Tractor", "Combine", "Feltzer",
 	"Remington", "Slamvan", "Blade", "Freight", "Streak", "Vortex", "Vincent",
	"Bullet", "Clover", "Sadler", "Firetruck", "Hustler", "Intruder", "Primo",
	"Cargobob", "Tampa", "Sunrise", "Merit", "Utility", "Nevada", "Yosemite",
	"Windsor", "Monster", "Monster", "Uranus", "Jester", "Sultan", "Stratum",
	"Elegy", "Raindance", "RC Tiger", "Flash", "Tahoma", "Savanna", "Bandito",
	"Freight Flat", "Streak Carriage", "Kart", "Mower", "Dune", "Sweeper",
	"Broadway", "Tornado", "AT-400", "DFT-30", "Huntley", "Stafford", "BF-400",
	"News Van", "Tug", "Trailer", "Emperor", "Wayfarer", "Euros", "Hotdog", "Club",
	"Freight Box", "Trailer", "Andromada", "Dodo", "RC Cam", "Launch", "Police Car",
 	"Police Car", "Police Car", "Police Ranger", "Picador", "S.W.A.T", "Alpha",
 	"Phoenix", "Glendale", "Sadler", "Luggage", "Luggage", "Stairs", "Boxville",
 	"Tiller", "Utility Trailer"
};
new Peds[115][1] = {
	{1},{2},{6},{7},{9},{10},{14},{15},{16},{18},{19},{20},{21},{22},{23},{24},{25},{26},{27},{28},
	{29},{30},{31},{32},{33},{34},{35},{36},{37},{38},{39},{41},{43},{44},{45},{47},{48},{49},{51},
	{52},{53},{54},{63},{64},{66},{67},{68},{69},{73},{75},{77},{78},{79},{80},{81},{82},{83},{84},
	{87},{88},{89},{94},{95},{96},{97},{99},{100},{101},{128},{129},{130},{131},{132},{133},{134},
	{135},{136},{137},{138},{139},{140},{142},{143},{144},{145},{146},{154},{162},{170},{180},{177},
	{176},{181},{182},{183},{184},{193},{201},{202},{203},{204},{221},{230},{234},{237},{241},{242},
	{247},{248},{249},{250},{252},{254},{299},{293}
};