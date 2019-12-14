#include <a_samp>
#include <easyDialog>
Dialog:DIALOG_FINDCAR(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	new Float:vehx, Float:vehy, Float:vehz, idd = PlayerInfo[playerid][pCarKey][listitem];
	GetVehiclePos(CarInfo[idd][Spawned], vehx, vehy, vehz);
	SetPlayerCheckpointEx(playerid, vehx, vehy, vehz, 3.0);
	CP[playerid] = 53;
	return 1;
}
Dialog:DIALOG_TOWCAR(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	new idd = PlayerInfo[playerid][pCarKey][listitem];
	if(PlayerMoney(playerid, 1000)) 
		return SCM(playerid, COLOR_LGREEN, "Eroare: Nu ai $1.000!");
	
	
	if(IsVehicleOccupied(CarInfo[idd][Spawned])) 
		return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau este ocupat!");
	
	if(CarInfo[idd][Confiscated] == 1) 
		return SCM(playerid, -1, "Acest vehicul este confiscat deoarece a fost parcat neregulamentar! Plateste amenda (ultimul rand din meniu).");
	
	if(IsAttached(CarInfo[idd][Spawned])) 
		return SCM(playerid, COLOR_LGREEN, "Eroare: Nu poti folosi aceasta functie momentan!");
	
	SetVehicleToRespawn(CarInfo[idd][Spawned]);
	SetVehicleHealth(CarInfo[idd][Spawned], CarInfo[idd][cVip] ? 2000 : 1000);
	SetVehicleVirtualWorld(CarInfo[idd][Spawned], CarInfo[idd][cVirtual]);
	CarInfo[idd][cTime] = gettime()+600;
	SetTunning(idd);
	GivePlayerCash(playerid, 0, 1000);
	format(gString, 65, "Your %s has been respawned.", aVehicleNames[CarInfo[idd][cModel]-400]);
	SCM(playerid, COLOR_LIGHTBLUE, gString);
	return 1;
}
Dialog:DIALOG_NEON(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	listitemm[playerid] = listitem;

	Dialog_Show(playerid, DIALOG_BUYNEON, DIALOG_STYLE_MSGBOX, "Neon", "Esti sigur ca vrei sa achizitionezi aceste neoane?", "Da", "Nu");
	return 1;
}
Dialog:DIALOG_BUYNEON(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	new idd = PersonalCar(playerid);
	if(idd == -1)
		return true;

	if(PlayerInfo[playerid][pPremiumPoints] < ((listitemm[playerid]+1)*100)+69)
		return SCM(playerid, COLOR_WHITE, "Nu ai aceasta suma de puncte premium.");

	if(CarInfo[idd][cNeon] != 0) {
		DestroyDynamicObject(NeonObj[0][CarInfo[idd][Spawned]]);
		DestroyDynamicObject(NeonObj[1][CarInfo[idd][Spawned]]);
	}

	PlayerInfo[playerid][pPremiumPoints] -= ((listitemm[playerid]+1)*100)+69;
	UpdateVar(playerid, "GoldPoints", PlayerInfo[playerid][pPremiumPoints]);

	CarInfo[idd][cNeon] = listitemm[playerid]+1;
	gQuery[0] = EOS;
	mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `cars` SET `Neon`='%d' WHERE `ID`='%d'", listitemm[playerid]+1, CarInfo[idd][cID]);
	mysql_tquery(SQL, gQuery);

	AttachNeon(idd);

	format(gString, 150, "Neons have been attached to your vehicle (-%d69 premium points)", listitemm[playerid]+1);
	SCM(playerid, COLOR_WHITE, gString);
	return 1;
}
Dialog:DIALOG_BETRED(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	if(strval(inputtext) <= 0 || strval(inputtext) > 100000000)
		return SCM(playerid, COLOR_GREY, "Invalid Amount.");

	if(GetPlayerCash(playerid) < strval(inputtext))
		return SCM(playerid, COLOR_GREY, "You don't have this amount of money");

	GivePlayerCash(playerid, 0, strval(inputtext));
	PlaceBet[playerid][0] = strval(inputtext);
	TotalBetPlace[0] ++;
	PrizePlace[0] += strval(inputtext);
	format(gString, 50, "%d bets~n~$%d", TotalBetPlace[0], PrizePlace[0]);
	TextDrawSetString(RoulettaTD[16], gString);
	format(gString, 55, "Ai pariat suma de $%s pe {FF0000}RED (WIN 2X)", FormatNumber(PlaceBet[playerid][0]));
	SCM(playerid, COLOR_YELLOW, gString);
	foreach(new i : IsRuletta) {
		if(i == playerid) continue;
		
		format(gString, 75, "%s a pariat suma de $%s pe RED (WIN 2X)", GetName(playerid), FormatNumber(PlaceBet[playerid][0]));
		SCM(i, COLOR_RED, gString);
	}
	PlaceAlerdy[playerid] = 1;
	StartRulettaIn();
	return 1;
}					
Dialog:DIALOG_BETGREEN(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	if(strval(inputtext) <= 0 || strval(inputtext) > 100000000)
		return SCM(playerid, COLOR_GREY, "Invalid Amount.");

	if(GetPlayerCash(playerid) < strval(inputtext))
		return SCM(playerid, COLOR_GREY, "You don't have this amount of money.");

	GivePlayerCash(playerid, 0, strval(inputtext));
	PlaceBet[playerid][1] = strval(inputtext);
	TotalBetPlace[1] ++;
	PrizePlace[1] += strval(inputtext);
	format(gString, 50, "%d bets~n~$%d", TotalBetPlace[1], PrizePlace[1]);
	TextDrawSetString(RoulettaTD[17], gString);
	format(gString, 55, "Ai pariat suma de $%s pe {65ec32}GREEN (WIN 14X)", FormatNumber(PlaceBet[playerid][1]));
	SCM(playerid, COLOR_YELLOW, gString);
	foreach(new i : IsRuletta) {
		if(i == playerid) continue;
		
		format(gString, 75, "%s a pariat suma de $%s pe GREEN (WIN 14X)", GetName(playerid), FormatNumber(PlaceBet[playerid][1]));
		SCM(i, 0x65ec32FF, gString);
	}
	PlaceAlerdy[playerid] = 2;
	StartRulettaIn();
	return 1;
}
Dialog:DIALOG_BETBLACK(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	if(strval(inputtext) <= 0 || strval(inputtext) > 100000000)
		return SCM(playerid, COLOR_GREY, "Invalid Amount.");

	if(GetPlayerCash(playerid) < strval(inputtext))
		return SCM(playerid, COLOR_GREY, "You don't have this amount of money");

	GivePlayerCash(playerid, 0, strval(inputtext));
	PlaceBet[playerid][2] = strval(inputtext);
	TotalBetPlace[2] ++;
	PrizePlace[2] += strval(inputtext);
	format(gString, 50, "%d bets~n~$%d", TotalBetPlace[2], PrizePlace[2]);
	TextDrawSetString(RoulettaTD[18], gString);
	format(gString, 55, "Ai pariat suma de $%s pe {656665}BLACK (WIN 2X)", FormatNumber(PlaceBet[playerid][2]));
	SCM(playerid, COLOR_YELLOW, gString);
	foreach(new i : IsRuletta) {
		if(i == playerid) continue;
		
		format(gString, 75, "%s a pariat suma de $%s pe {656665}BLACK (WIN 2X)", GetName(playerid), FormatNumber(PlaceBet[playerid][2]));
		SCM(i, COLOR_GREY, gString);
	}
	PlaceAlerdy[playerid] = 3;
	StartRulettaIn();
	return 1;
}
Dialog:DIALOG_SETGUN(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	PlayerInfo[playerid][pGuns][listitem] = PlayerInfo[playerid][pGuns][listitem] ? false : true;

	format(gString, 60, "(SET GUN): {FFFFFF}guns %s set %s", GunMafie[listitem], PlayerInfo[playerid][pGuns][listitem] ? ("{00ff00}Enabled") : ("{ff0000}Desabled"));
	SCM(playerid, COLOR_LGREEN, gString);
	save_guns(playerid);
	return 1;
}
Dialog:DIALOG_VIPNAME(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	new idd = PersonalCar(playerid);
	switch(listitem) {
		case 0: Dialog_Show(playerid, DIALOG_VIPNAME1, DIALOG_STYLE_INPUT, "{FCD482}VIP {FFFFFF}Menu", "Introdu name text-ul:", "Select", "Cancel");
		case 1: Dialog_Show(playerid, DIALOG_VIPNAME1, DIALOG_STYLE_LIST, "{FCD482}VIP {FFFFFF}Menu", "{FFFFFF}White\n{0BA10B}Dark green\n{D1FF99}Light green\n{FF3D98}Pink\n{F9FF3D}Yellow\n{FFEF85}Light yellow\n{3DE5FF}Lightblue\n{FFAB3D}Orange\n{A13DFF}Mauve\n{FFC27D}Gold\n{FFA3A3}Light red\n{C5A3FF}Purple", "Select", "Cancel");
		case 2: {
			DestroyDynamicObject(nameobject[CarInfo[idd][Spawned]]);
			EditVipCar(playerid, idd);
			SCM(playerid, COLOR_LIGHTGOLD, "VIP MENU: {FFFFFF}Foloseste-te de coodonatele (X, Y, Z) pentru a muta textul.");
		}
		case 3: {
			for(new i = 0; i < 5; i++) CarInfo[idd][cVipPos][i] = 0;
			DestroyDynamicObject(nameobject[CarInfo[idd][Spawned]]);
			nameobject[CarInfo[idd][Spawned]] = CreateDynamicObject(19327, CarInfo[idd][cVipPos][0], CarInfo[idd][cVipPos][1], CarInfo[idd][cVipPos][2], CarInfo[idd][cVipPos][3], CarInfo[idd][cVipPos][4], CarInfo[idd][cVipPos][5]);
			SetDynamicObjectMaterialText(nameobject[CarInfo[idd][Spawned]], 0, CarInfo[idd][cText], OBJECT_MATERIAL_SIZE_256x128, "Arial", 25, 1, -1, 0, 1);
			Save_VipCoordonat(idd);
			SCM(playerid, COLOR_LIGHTGOLD, "VIP NAME: {FFFFFF}text-ul de pe masina a fost resetat.");
		}
	}
	listitemm[playerid] = listitem;
	return 1;
}
Dialog:DIALOG_VIPNAME1(playerid, response, listitem, inputtext[]) {
	if(!response) 
		return true;
	new idd = PersonalCar(playerid);
	switch(listitemm[playerid]) {
		case 0: {
			if(strlen(inputtext) < 3 || strlen(inputtext) > 32) {
				SCM(playerid, COLOR_LGREEN, "Eroare: Poti introduce intre 3 si 32 de caractere!");
				Dialog_Show(playerid, DIALOG_VIPNAME1, DIALOG_STYLE_INPUT, "{FCD482}VIP {FFFFFF}Menu", "Introdu name text-ul:", "Select", "Cancel");
				return true;
			}
			DestroyDynamicObject(nameobject[CarInfo[idd][Spawned]]);
			format(CarInfo[idd][cText], 32, inputtext);
			mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `cars` SET `Text`='%s' WHERE `ID`='%d'", CarInfo[idd][cText], CarInfo[idd][cID]);
			mysql_tquery(SQL, gQuery, "", "");
			attach_vip_text(idd);
			SCMf(playerid, COLOR_LIGHTGOLD, "VIP MENU: {FFFFFF}Noul text este {%s}%s", serverColors(CarInfo[idd][cTextColor]), CarInfo[idd][cText]);
		}
		case 1: {
			DestroyDynamicObject(nameobject[CarInfo[idd][Spawned]]);
			CarInfo[idd][cTextColor] = listitem;
			mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `cars` SET `ColorText`='%d' WHERE `ID`='%d'", listitem, CarInfo[idd][cID]);
			mysql_tquery(SQL, gQuery, "", "");
			SCMf(playerid, COLOR_LIGHTGOLD, "VIP MENU: {FFFFFF}Noua culoare este {%s}%s", serverColors(CarInfo[idd][cTextColor]), CarInfo[idd][cText]);
			attach_vip_text(idd);
		}
	}
	return 1;
}
Dialog:DIALOG_RENTCAR(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(RentStock[listitem] == 0) return SCM(playerid, COLOR_GREY, "Acest vehicul nu mai este in stock");
	new string[144];
	RentStock[listitem] --;
	new rand = random(8);
	VehicleRent[playerid] = CreateVehicleEx(RentModel[0][listitem], RentSpawn[rand][0], RentSpawn[rand][1], RentSpawn[rand][2], RentSpawn[rand][3], random(255), random(255), -1, 0);
	format(string, sizeof(string), "Ai inchiriat vehiculul %s, vei plati $150 la fiecare 10 secunde daca foloseti vehiculul.", aVehicleNames[RentModel[0][listitem]-400]);
	SCM(playerid, COLOR_YELLOW, string);
	SCM(playerid, COLOR_YELLOW, "Daca doresti sa renunti la vehiculul inchiriat, ai la dispozitie comanda (/unrent).");
	Gas[VehicleRent[playerid]] = 100;
	IsRentCar[playerid] ++;
	SetVehicleNumberPlate(VehicleRent[playerid], "Rent Car");
	GivePlayerCash(playerid, 0, 2000);
	PutPlayerInVehicleEx(playerid, VehicleRent[playerid], 0);
	new engine,lights,alarm,doors,bonnet,boot,objective;
	GetVehicleParamsEx(VehicleRent[playerid],engine,lights,alarm,doors,bonnet,boot,objective);
	SetVehicleParamsEx(VehicleRent[playerid],engine,lights,alarm,0,bonnet,boot,objective);
	return 1;
}
Dialog:DIALOG_BETP(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new money = strval(inputtext);
	if(money < 500000 || money > 5000000) return SCM(playerid, COLOR_GREY, "Invalid.");
	if(PlayerMoney(playerid, money)) return SCM(playerid, COLOR_LIGHTGREEN, "Nu ai bani introdusi.");
	new string[144];
	format(string, sizeof(string), "Bet: $%s", FormatNumbers(money));
	PlayerTextDrawSetString(playerid, PacaneleTD[8], string);
	BetPacanele[playerid] = money;
	return 1;
}
Dialog:DIALOG_GIVECRATES(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new id = GetPVarInt(playerid, "id");
	if(!IsPlayerConnected(id))
		return true;

	format(gString, 100, "(/givecrate) {FFFFFF}%s i-a dat lui %s un %s crate.", GetName(playerid), GetName(id), CrateName[listitem]);
	SendAdminMessage(COLOR_RED, gString, 6);
	format(gString, 100, "Admin {FF0000}%s{FFFFFF} ti-a dat un crate %s.", GetName(playerid), CrateName[listitem]);
	SCM(id, COLOR_WHITE, gString);
	PlayerInfo[id][pCrates][listitem] ++;
	save_crates(id);
	return 1;
}
Dialog:DIALOG_CRATES(playerid, response, listitem, inputtext[]) {
	if(!response) return ShowCrates(playerid);
	if(PlayerInfo[playerid][pCrates][listitem] < 1) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu ai o cuite de acest tip"), ShowCrates(playerid);
	format(gString, 75, "You have opened a {%s}%s crates{FFFFFF}! Waiting...", CrateColors[listitem], CrateName[listitem]);
	SCM(playerid, COLOR_WHITE, gString);
	PlayerInfo[playerid][pCrates][listitem] --;
	save_crates(playerid);
	CrateTime[playerid] = 20;
	CrateModel[playerid] = listitem+1;
	startcrates(playerid);
	return 1;
}
Dialog:DIALOG_SKINS(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new x = GetPVarInt(playerid, "skin"), id = PlayerInfo[playerid][pSkins][x];
	if(GetPVarInt(playerid, "skin") == 0) return SCM(playerid, COLOR_LGREEN, "Nu poti sterge skin-ul de civil");
	if(GetPlayerSkin(playerid) == id) {
		PlayerInfo[playerid][pChar] = 250;
		SetPlayerSkinEx(playerid, 250);
		UpdateVar(playerid, "CChar", id);
	}
	new string[144];
	format(string, sizeof(string), "{DF80F9}(Skin) {FFFFFF}Skin #%d sters. (model: %d | type: %s)", x+1, PlayerInfo[playerid][pSkins][x], GetSkinType(PlayerInfo[playerid][pSkins][x]));
	SendClientMessage(playerid, COLOR_WHITE, string);
	PlayerInfo[playerid][pSkins][x] = -1;
   	PlayerTextDrawSetPreviewModel(playerid, SkinsPTD[x], 18631);
   	PlayerTextDrawColor(playerid, SkinsPTD[x], 0xFFFFFFFF);
   	PlayerTextDrawBackgroundColor(playerid, SkinsPTD[x], 0xFFFFFF20);
   	PlayerTextDrawShow(playerid, SkinsPTD[x]);
	save_skin(playerid);
	DeletePVar(playerid, "skin");
	return 1;
}

Dialog:DIALOG_SKIN(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new x = GetPVarInt(playerid, "skin"), id = PlayerInfo[playerid][pSkins][x];
	PlayerInfo[playerid][pChar] = id;
	SetPlayerSkinEx(playerid, id);
	UpdateVar(playerid, "CChar", id);
	new string[144];
	format(string, sizeof(string), "{DF80F9}(Skin) {FFFFFF}Skin #%d select. (model: %d | Type: %s)", GetPVarInt(playerid, "skin")+1, id, GetSkinType(id));
	SendClientMessage(playerid, COLOR_WHITE, string);
	DeletePVar(playerid, "skin");
	SkinsActions[playerid] = 0;
	IsSkins[playerid] = 0;
	CancelSelectTextDraw(playerid);
	for(new i = 0; i < 10; i++) PlayerTextDrawHide(playerid, SkinsPTD[i]);
	for(new i = 0; i < 9; i++) TextDrawHideForPlayer(playerid, SkinsTD[i]);
	return 1;
}

Dialog:DIALOG_PET(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	new nxtlevel = PlayerInfo[playerid][pPetLevel], expamount;
	expamount = nxtlevel*PetLevelExp;
	new string[144];			
	switch(listitem) {
		case 0: {
			if(PlayerInfo[playerid][pPetPoints] < expamount) return SCM(playerid,COLOR_GRAD1,"Nu ai destule Pet Points.");
			PlayerInfo[playerid][pPetLevel]++;
			PlayerInfo[playerid][pPetPoints] -= expamount;
			format(string, sizeof(string), "Felicitari! Acum, Pet-ul tau are level %d.",PlayerInfo[playerid][pPetLevel]);

			SCM(playerid,COLOR_LIGHTBLUE,string);

			new total = PlayerInfo[playerid][pPetPoints];
			if(total > 0) PlayerInfo[playerid][pPetPoints] = total;
			else PlayerInfo[playerid][pPetPoints] = 0;
			Update(playerid, pPetPointsx);
			Update(playerid, pPetLevelx);
		} case 1: {
			if(!response) return true;
			// cow // SetPlayerAttachedObject(playerid, 8, 16442, 1, 0.360000, 0.011000, 0.137000, 93.400009, 85.899986, 11.000001, 0.053000, 0.062999, 0.040999);
		} case 2: {
			if(!response) return true;
			if(PlayerInfo[playerid][pPetStatus] == 0) {
				PlayerInfo[playerid][pPetStatus] = 1;
				SetPlayerAttachedObject(playerid, 6, 19078, 17, 0.021065, -0.100726, 0.143000, 357.120239, 1.712298, 349.232513, 1.000000, 1.000000, 1.120420);
				format(string, sizeof(string), "{F08A23}Pet level.{EA2323}%d - %s",PlayerInfo[playerid][pPetLevel], PlayerInfo[playerid][pPetName]);
				Pet[playerid] = CreateDynamic3DTextLabel(string, 0xFFFFFF00, 357.120239, 1.712298, 349.232513, 20, playerid);
				SCM(playerid, COLOR_LIGHTGREEN, "Pet: Enable");
				Update(playerid, pPetStatusx);
			}
			else {
				RemovePet(playerid);
				SCM(playerid, COLOR_LIGHTGREEN, "Pet: Disable");
				PlayerInfo[playerid][pPetStatus] = 0;
				Update(playerid, pPetStatusx);
			}
		} case 3: {
			if(!response) return true;
			if(PlayerInfo[playerid][pPetLevel] < 21) return true;
			if(strlen(PlayerInfo[playerid][pPetName]) > 0)
				format(string, sizeof(string), "Numele actual al pet-ului este %s.\nDaca doresti sa il schimbi, te rugam sa introduci numele dorit in casuta de mai jos.", PlayerInfo[playerid][pPetName]);
			if(strlen(PlayerInfo[playerid][pPetName]) == 0)
				format(string, sizeof(string), "Nu ai un nume pe pet.\nDaca doresti sa iti pui un nume, te rugam sa introduci numele dorit in casuta de mai jos.", PlayerInfo[playerid][pPetName]);
			Dialog_Show(playerid, DIALOG_PETNAME, DIALOG_STYLE_INPUT, "Pet name change", string, "Change", "Cancel");
		} case 4: {
			Checkstatuspet(playerid);
		}
	}	
	return 1;
}
Dialog:DIALOG_PETNAME(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new string[144];
	if(!isnull(inputtext)) {
		if(strlen(inputtext) >= 5 && strlen(inputtext) <= 15) {
			DestroyDynamic3DTextLabel(Pet[playerid]);	
			format(string, sizeof(string),"UPDATE `users` SET `PetName` = '%s' WHERE `Name` = '%s'",inputtext, GetName(playerid));
			mysql_tquery(SQL, string, "", "");
			format(PlayerInfo[playerid][pPetName], 30, inputtext);
			SCMEx(playerid, -1, "Ai schimbat numele pet-ului in %s",PlayerInfo[playerid][pPetName]);
			Update(playerid, pPetNamex);	
			PlayerInfo[playerid][pPremiumPoints] -= 20;
			Update(playerid, pPremiumPointsx);

			format(string, sizeof(string), " {6AD439}%s{FFFFFF} - level {6AD439}%d{FFFFFF}",PlayerInfo[playerid][pPetName],PlayerInfo[playerid][pPetLevel]);
			Pet[playerid] = CreateDynamic3DTextLabel(string, 0xFFFFFF00, 357.120239, 1.712298, 349.232513, 20, playerid);
		}
		else {
			SCM(playerid, COLOR_YELLOW, "Eroare: Nume pet invalid. (5 - 15 litere)");
			format(string, sizeof(string), "Numele actual al pet-ului este %s\nDaca doresti sa il schimbi te rugam introdu numele dorit in casuta de mai jos\nNumele trebuie sa fie de minin 5 caractere si maxim 15",PlayerInfo[playerid][pPetName]);
			Dialog_Show(playerid, DIALOG_PETNAME, DIALOG_STYLE_INPUT, "Change pet name", string, "Change", "Cancel");	
		}
	}
	else {
		SCM(playerid, COLOR_YELLOW, "Eroare: Nume pet invalid. (5 - 15 litere)");
		format(string, sizeof(string), "Numele actual al pet-ului este %s\nDaca doresti sa il schimbi te rugam introdu numele dorit in casuta de mai jos\n Numele trebuie sa fie de minin 5 caractere si maxim 15",PlayerInfo[playerid][pPetName]);
		Dialog_Show(playerid, DIALOG_PETNAME, DIALOG_STYLE_INPUT, "Change pet name", string, "Change", "Cancel");
	}
	return 1;
}		

Dialog:DIALOG_REPORT(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	SetPVarInt(playerid, "Selected", listitem);
	switch(listitem) {
		case 0: Dialog_Show(playerid, DIALOG_REPORT1, DIALOG_STYLE_INPUT, "Raporteaza DM", "Scrie ID-ul sau numele jucatorului pe care vrei sa-l raportezi:", "Next", "Back");
		case 1: Dialog_Show(playerid, DIALOG_REPORT1, DIALOG_STYLE_INPUT, "Raporteaza un codat", "Scrie ID-ul sau numele jucatorului pe care vrei sa-l raportezi:", "Next", "Back");
		case 2: {
			Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Intrebari despre donatii", 
			"Cum donez?\nPentru a dona, trebuie sa iei legatura cu ownerii serverului", "Close", "");
		}	
		case 3: {
			new string[144];
			format(string, sizeof(string), "%s (%d) este blocat.", GetName(playerid), playerid);
			SendAdminMessage(0xC9161FFF, string, 1);
				
			if(ReportTimer[playerid] != 0) {
				KillTimer(ReportTimer[playerid]);
				ReportTimer[playerid] = 0;
			}			
			
			ReportTimer[playerid] = SetTimerEx("CheckReport", 300000, 0, "%d", playerid);
			SCM(playerid, COLOR_YELLOW, "Reportul tau a fost trimis adminilor.");
			PlayerInfo[playerid][pReportTime] = 180;

			SetPVarInt(playerid, "Reported", 1);
			SetPVarInt(playerid, "ReportType", 3);			
		}	
		case 4: Dialog_Show(playerid, DIALOG_REPORT1, DIALOG_STYLE_INPUT, "Alta problema", "Scrie problema pe care o ai.\nNu folosi comanda /report aiurea deoarece vei fi sanctionat.", "Ok", "Back");
	}
	return 1;
}

Dialog:DIALOG_REPORT1(playerid, response, listitem, inputtext[]) {
	if(!response) return Dialog_Show(playerid, DIALOG_REPORT, DIALOG_STYLE_LIST, "Report", "Raporteaza DM\nRaporteaza un codat\nIntrebari despre donatii\nSunt blocat\nAlta problema", "Select", "Close");
	new type = GetPVarInt(playerid, "Selected"), id = strval(inputtext);
	new string[144];
	if(type != 4) {
		if(sscanf(inputtext, "u", id)) {
			Dialog_Show(playerid, DIALOG_REPORT, DIALOG_STYLE_LIST, "Report", "Raporteaza DM\nRaporteaza un codat\nIntrebari despre donatii\nSunt blocat\nAlta problema", "Select", "Close");
			SCM(playerid, COLOR_GRAD3, "ID Invalid!");
			return true;
		}
		if(id == playerid) {
			Dialog_Show(playerid, DIALOG_REPORT, DIALOG_STYLE_LIST, "Report", "Raporteaza DM\nRaporteaza un codat\nIntrebari despre donatii\nSunt blocat\nAlta problema", "Select", "Close");
			SCM(playerid, COLOR_GRAD4, "Nu te poti adauga pe tine in lista!");
			return true;
		}	
		if(!IsPlayerConnected(id)) {
			Dialog_Show(playerid, DIALOG_REPORT, DIALOG_STYLE_LIST, "Report", "Raporteaza DM\nRaporteaza un codat\nIntrebari despre donatii\nSunt blocat\nAlta problema", "Select", "Close");
			SCM(playerid, COLOR_GRAD4, "Acel player nu este online!");
			return true;
		}
	}
	
	switch(type) {
		case 0: {
			format(string, sizeof(string), "%s (%d) l-a raportat pe %s (%d) pentru DM.", GetName(playerid), playerid, GetName(id), id);
			SendAdminMessage(0xC9161FFF, string, 1);
			
			SetPVarInt(id, "Reported", 1);
			SetPVarInt(id, "ReportType", 1);
			SetPVarString(id, "ReportedBy", GetName(playerid));
			SetPVarInt(id, "ReportScore", GetPVarInt(id, "ReportScore")+1);
		}
		case 1: {
			format(string, sizeof(string), "%s (%d) l-a raportat pe %s (%d) pentru cheats.", GetName(playerid), playerid, GetName(id), id);
			SendAdminMessage(0xC9161FFF, string, 1);

			SetPVarInt(id, "Reported", 1);
			SetPVarInt(id, "ReportType", 2);
			SetPVarString(id, "ReportedBy", GetName(playerid));
			SetPVarInt(id, "ReportScore", GetPVarInt(id, "ReportScore")+1);			
		}
		case 4: {
			if(CuvinteCheie(inputtext)) {
				new time = 2;
				PlayerInfo[playerid][pReportTime] += time*60;
				Update(playerid, pReportTimex);
				format(string, sizeof(string), "Ai primit mute pe /report pentru %d minute! Motiv: Report aiurea.", time);
				SCM(playerid, -1, string);
				format(string, sizeof(string), "AdmBot: %s a primit mute %d minute pe /report de la AdmBot, motiv: Report aiurea.", GetName(playerid), time);
				SCMTA(COLOR_LIGHTRED, string);
				return true;
			}	
			else {
				format(string, sizeof(string), "Report de la %s (ID: %d, lvl: %d): %s.", GetName(playerid), playerid, PlayerInfo[playerid][pLevel], inputtext);
				SendAdminMessage(0xC9161FFF, string, 1);
				SetPVarInt(playerid, "Reported", 1);
				SetPVarInt(playerid, "ReportType", 4);						
				SetPVarString(playerid, "ReportText", inputtext);				
			}		
		}
	}
	if(ReportTimer[playerid] != 0) {
		KillTimer(ReportTimer[playerid]);
		ReportTimer[playerid] = 0;
	}
			
	PlayerInfo[playerid][pReportTime] = 180;
	Ajutor[playerid] = 1;
	ReportTimer[playerid] = SetTimerEx("CheckReport", 300000, 0, "%d", playerid);
	SCM(playerid, COLOR_YELLOW, "Reportul tau a fost trimis adminilor.");
	return 1;
}

Dialog:DIALOG_EDITVEHICLE(playerid, response, listitem, inputtext[]) {
	if(!response) return SCM(playerid, COLOR_LGREEN, "Ai inchis dialogul!");
	switch(listitem) {
		case 0: Dialog_Show(playerid, DIALOG_EDITVEHICLE1, DIALOG_STYLE_INPUT, "Edit Value", "Scrie mai jos valoarea in pe care vrei sa modifici!", "Ok", "Exit");
		case 1: Dialog_Show(playerid, DIALOG_EDITVEHICLE1, DIALOG_STYLE_INPUT, "Edit Stock", "Scrie mai jos valoarea in pe care vrei sa modifici!", "Ok", "Exit");	
	}
	SetPVarInt(playerid, "listitem", listitem);
	return 1;
}
Dialog:DIALOG_EDITVEHICLE1(playerid, response, listitem, inputtext[]) {
	if(!response) return SCM(playerid, COLOR_LGREEN, "Ai inchis dialogul!");
	listitem = GetPVarInt(playerid, "listitem");
	new idd = GetPVarInt(playerid, "VehicleID");
	new string[144];
	switch(listitem) {
		case 0: {
			DSModels[idd][dPrice] = strval(inputtext);
			format(string, sizeof(string), "Noul pret al acestui vehicul este acum $%s!",FormatNumber(strval(inputtext)));
			SCM(playerid, COLOR_LGREEN, string);
			mysql_format(SQL, string, sizeof(string), "UPDATE `dsveh` SET `Price`='%d' WHERE `ID`='%d'", DSModels[idd][dPrice], idd);
			mysql_tquery(SQL, string, "", "");
		}
		case 1: {
			DSModels[idd][dStock] = strval(inputtext);
			format(string, sizeof(string), "Stockurile la acest model au fost modificate in %d!",strval(inputtext));
			SCM(playerid, COLOR_LGREEN, string);
			mysql_format(SQL, string, sizeof(string), "UPDATE `dsveh` SET `Stock`='%d' WHERE `ID`='%d'", DSModels[idd][dStock], idd);
			mysql_tquery(SQL, string, "", "");
		}			
	}
	return 1;
}

Dialog:DIALOG_CALLS(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new id = SelectedPlayers[playerid][listitem];
	new string[144], sendername[MAX_PLAYER_NAME];
	if(!IsPlayerConnected(id) || id == INVALID_PLAYER_ID) return SCM(playerid, COLOR_GREY, "Acel player nu este conectat!");
	switch(PlayerInfo[playerid][pMember]) {
		case 12: {
			if(TaxiService[0][id] == 1 && AcceptedTaxiService[0][id] == 0) {
				if(PlayerInfo[playerid][pMember] != 12) return true;
				if(TaxiService[0][id] == 0) return SCM(playerid, COLOR_GREY, "Acel player nu a apelat la acest serviciu!");
				if(AcceptedService[playerid] != -1) return SCM(playerid, COLOR_GREY, "Ai acceptat deja o comanda, pentru a renunta la cea curenta, foloseste '/cancel command'.");
				if(AcceptedTaxiService[0][id] == 1) return SCM(playerid, COLOR_GREY, "Comanda acelui player a fost acceptat de catre alt membru al factiunii.");
				if(!FactionVehicle(playerid)) return SCM(playerid, COLOR_GREY, "Nu esti intr-un vehicul ce apartine factiunii tale!");	
				AcceptedTaxiService[0][id] = 1;
				AcceptedService[playerid] = id;
				targetfind[playerid] = id;
				UsedFind[playerid] = 1;
				format(string, sizeof(string), "**(( Taxi Dispatch: Taximetristul %s (%d) i-a acceptat comanda lui %s (%d). ))**", GetName(playerid), playerid, GetName(id), id);
				SendFactionMessage(12, 0x2C6CBFFf, string);
				format(string, sizeof(string), "* Taximetristul %s ti-a preluat comanda.", GetName(playerid));
				SCM(id, COLOR_YELLOW, string);	
				CP[playerid] = 53;
			}
			else return SCM(playerid, COLOR_LGREEN, "Eroare: Acel player nu a apelat la serviciile factiunii tale sau comanda lui a fost accepta de altcineva!");
		}
		case 15: {
			if(TaxiService[1][id] == 1 && AcceptedTaxiService[1][id] == 0) {
				if(PlayerInfo[playerid][pMember] != 15) return true;
				if(TaxiService[1][id] == 0) return SCM(playerid, COLOR_GREY, "Acel player nu a apelat la acest serviciu!");
				if(AcceptedService[playerid] != -1) return SCM(playerid, COLOR_GREY, "Ai acceptat deja o comanda, pentru a renunta la cea curenta, foloseste '/cancel command'.");
				if(AcceptedTaxiService[1][id] == 1) return SCM(playerid, COLOR_GREY, "Comanda acelui player a fost acceptat de catre alt membru al factiunii.");
				if(!FactionVehicle(playerid)) return SCM(playerid, COLOR_GREY, "Nu esti intr-un vehicul ce apartine factiunii tale!");	
				AcceptedTaxiService[1][id] = 1;
				AcceptedService[playerid] = id;
				targetfind[playerid] = id;
				UsedFind[playerid] = 1;
				format(string, sizeof(string), "**(( Uber Dispatch: Uberul %s (%d) i-a acceptat comanda lui %s (%d). ))**", GetName(playerid), playerid, GetName(id), id);
				SendFactionMessage(15, 0x2C6CBFFf, string);
				format(string, sizeof(string), "* Uberul %s ti-a preluat comanda.", GetName(playerid));
				SCM(id, COLOR_YELLOW, string);	
				CP[playerid] = 53;
			}
			else return SCM(playerid, COLOR_LGREEN, "Eroare: Acel player nu a apelat la serviciile factiunii tale sau comanda lui a fost accepta de altcineva!");
		}
		case 13: {
			if(MedicCall[id] == id) {
				if(PlayerInfo[playerid][pMember] != 13) return SCM(playerid, COLOR_WHITE, "Nu esti medic!");
				if(MedicCallTime[playerid] > 0) return SCM(playerid, COLOR_WHITE, "You have already accepted a medic call.");
				if(IsAnAmbulance(GetPlayerVehicleID(playerid))) return SCM(playerid,COLOR_WHITE, "You're not in ambulance car.");
				if(MedicCall[id] != id) return SCM(playerid,COLOR_WHITE, "This player has not called ambulance.");
				format(string, sizeof(string), "**(( Paramedic Dispatch: Paramedic %s (%d) i-a acceptat comanda lui %s (%d). ))**",GetName(playerid), playerid, GetName(id), id);
				SendFactionMessage(13, COLOR_LIGHTBLUE, string);
				format(string, sizeof(string), "* Medicul %s ti-a acceptat comanda.",sendername);
				SCM(MedicCall[id], COLOR_YELLOW, string);
				new Float:X,Float:Y,Float:Z;
				GetPlayerPos(MedicCall[id], X, Y, Z);
				SetPlayerCheckpointEx(playerid, X, Y, Z, 5);
				MedicCallTime[playerid] = 1;
				MedicCall[id] = -1;
				CP[playerid] = 53;				
			}
			else return SCM(playerid, COLOR_LGREEN, "Eroare: Acel player nu a apelat la serviciile factiunii tale sau comanda lui a fost accepta de altcineva!");
		}	
		case 8: {
			if(MechanicCall[id] == id) {
				if(CP[playerid] != 0) return Dialog_Show(playerid, DIALOG_CHECKPOINT, DIALOG_STYLE_MSGBOX, "Checkpoint", "Ai deja un checkpoint activ.\nDoresti sa-l anulezi? Daca da, apasa pe 'Ok'.", "Ok", "Exit");
				if(PlayerInfo[playerid][pMember] != 8) return SCM(playerid, COLOR_GREY, "Nu faci parte din factiunea mecanic.");
				if(MechanicCallTime[playerid] == 1) return SCM(playerid, COLOR_WHITE, "Ai acceptat deja un apel!");
				if(MechanicCall[id] != id) return SCM(playerid,COLOR_WHITE,"Acel player nu a apelat la acest serviciu!");
				format(string, sizeof(string), "**(( TTC Dispatch: Tow Truck Member %s (%d) i-a acceptat comanda lui %s (%d). ))**", GetName(playerid), playerid, GetName(id), id);
				SendFactionMessage(8, COLOR_LIGHTBLUE, string);
				format(string, sizeof(string), "* Mecanicul %s a acceptat apelul tau.",sendername);
				SCM(MechanicCall[id], COLOR_WHITE, string);
				new Float:X,Float:Y,Float:Z;
				GetPlayerPos(MechanicCall[id], X, Y, Z);
				SetPlayerCheckpointEx(playerid, X, Y, Z, 5);
				UsedFind[playerid] = 1;
				targetfind[playerid] = id;
				MechanicCall[id] = -1;
				CP[playerid] = 53;			
			}
			else return SCM(playerid, COLOR_LGREEN, "Eroare: Acel player nu a apelat la serviciile factiunii tale sau comanda lui a fost accepta de altcineva!");
		}
		case 7: {
			if(InstructorCall[id] == 1) {
				if(InstructorCall[id] == 0) return SCM(playerid, COLOR_WHITE, "Acel player nu a apelat la un instructor.");
				if(PlayerInfo[playerid][pMember] != 7) return SCM(playerid, COLOR_GREY, "Nu faci parte din factiunea car mechanic.");
				if(CP[playerid] != 0) return Dialog_Show(playerid, DIALOG_CHECKPOINT, DIALOG_STYLE_MSGBOX, "Checkpoint", "Ai deja un checkpoint activ.\nDoresti sa-l anulezi? Daca da, apasa pe 'Ok'.", "Ok", "Exit");
				format(string, sizeof(string), "**(( SI Dispatch: Instructor %s (%d) i-a acceptat comanda lui %s (%d). ))**", GetName(playerid), playerid, GetName(id), id);
				SendFactionMessage(7, COLOR_LIGHTBLUE, string);
				format(string, sizeof(string), "* I-ai acceptat apelul lui %s.",GetName(id));
				SCM(playerid, COLOR_WHITE, string);
				format(string, sizeof(string), "* Instructorul %s a acceptat apelul tau.",GetName(playerid));
				SCM(id, COLOR_YELLOW, string);
				new Float:X,Float:Y,Float:Z;
				GetPlayerPos(InstructorCall[id], X, Y, Z);
				SetPlayerCheckpointEx(playerid, X, Y, Z, 5);
				UsedFind[playerid] = 1;
				targetfind[playerid] = id;
				InstructorCall[id] = 0;
				CP[playerid] = 53;			
			}
			else return SCM(playerid, COLOR_LGREEN, "Eroare: Acel player nu a apelat la serviciile factiunii tale sau comanda lui a fost accepta de altcineva!");
		}		
	}	
	return 1;
}

Dialog:DIALOG_MOVEHOUSE(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new Float: Pos[3], id = GetPVarInt(playerid, "MoveID");
	GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
	switch(listitem) {
		case 0: {
			if(GetPlayerInterior(playerid) == 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu esti intr-un interior!");
			HouseInfo[id][hExitx] = Pos[0];
			HouseInfo[id][hExity] = Pos[1];
			HouseInfo[id][hExitz] = Pos[2];
			HouseInfo[id][hInterior] = GetPlayerInterior(playerid);
			SCM(playerid, COLOR_LGREEN, "Interiorul casei a fost modificat!");
			gQuery[0] = EOS;
			mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `houses` SET `Exitx`='%f', `Exity`='%f', `Exitz`='%f', `Interior`='%d' WHERE `ID`='%d'", Pos[0], Pos[1], Pos[2], GetPlayerInterior(playerid), HouseInfo[id][hID]);
			mysql_tquery(SQL, gQuery, "", "");
		}
		case 1: {
			new query[144];
			if(GetPlayerInterior(playerid) != 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Esti intr-un interior!");
			HouseInfo[id][hEntrancex] = Pos[0];
			HouseInfo[id][hEntrancey] = Pos[1];
			HouseInfo[id][hEntrancez] = Pos[2];
			SCM(playerid, COLOR_LGREEN, "Exteriorul casei a fost modificat!");
			DestroyDynamic3DTextLabel(HouseLabel[id]);
			HouseLabel[id] = CreateDynamic3DTextLabel("none", 0xFFFFFF00, HouseInfo[id][hEntrancex], HouseInfo[id][hEntrancey], HouseInfo[id][hEntrancez], 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1,-1, -1, 100.0);
			UpdateLabel(1, id);
			mysql_format(SQL, query, sizeof(query), "UPDATE `houses` SET `Entrancex`='%f', `Entrancey`='%f', `Entrancez`='%f' WHERE `ID`='%d'", Pos[0], Pos[1], Pos[2], HouseInfo[id][hID]);
			mysql_tquery(SQL, query, "", "");
		}
	}
	return 1;
}
Dialog:DIALOG_MOVEBIZ(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new Float: Pos[3], id = GetPVarInt(playerid, "MoveID");
	GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
	gQuery[0] = EOS;
	switch(listitem) {
		case 0: {
			if(GetPlayerInterior(playerid) == 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu esti intr-un interior!");
			BizzInfo[id][bExitX] = Pos[0];
			BizzInfo[id][bExitY] = Pos[1];
			BizzInfo[id][bExitZ] = Pos[2];
			BizzInfo[id][bInterior] = GetPlayerInterior(playerid);
			SCM(playerid, COLOR_LGREEN, "Interiorul afacerii a fost modificat!");
			UpdateLabel(2, id);
			mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `bizz` SET `Exitx`='%f', `Exity`='%f', `Exitz`='%f', `Interior`='%d' WHERE `ID`='%d'", Pos[0], Pos[1], Pos[2], GetPlayerInterior(playerid), BizzInfo[id][bID]);
			mysql_tquery(SQL, gQuery, "", "");			
		}
		case 1: {
			if(GetPlayerInterior(playerid) != 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Esti intr-un interior!");
			DestroyDynamicPickup(BizzInfo[id][bPickup]);
			BizzInfo[id][bEntranceX] = Pos[0];
			BizzInfo[id][bEntranceY] = Pos[1];
			BizzInfo[id][bEntranceZ] = Pos[2];
			BizzInfo[id][bPickup] = CreateDynamicPickup(BizzInfo[id][bType] != 19 ? 1239 : 1318, 1, BizzInfo[id][bEntranceX], BizzInfo[id][bEntranceY], BizzInfo[id][bEntranceZ], -1, -1, -1, 100.0);
			SCM(playerid, COLOR_LGREEN, "Exteriorul afacerii a fost modificat!");
			UpdateLabel(2, id);
			mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `bizz` SET `Entrancex`='%f', `Entrancey`='%f', `Entrancez`='%f' WHERE `ID`='%d'", Pos[0], Pos[1], Pos[2], BizzInfo[id][bID]);
			mysql_tquery(SQL, gQuery, "", "");		
		}
	}
	return 1;
}
Dialog:DIALOG_TELEPORT(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	switch(listitem) {
		case 0: {
			SetPlayerVirtualWorld(playerid, 1339);
			SetPlayerPosEx(false, playerid, 1772.0298,-1548.0693,9.9068);
			SetPlayerInterior(playerid, 0);				
		}
		case 1: {
			SetPlayerVirtualWorld(playerid, 0);
			SetPlayerPosEx(false, playerid, 1798.5109,-1578.3337,14.0848);
			SetPlayerInterior(playerid, 0);				
		}				
	}
	return 1;
}	
Dialog:DIALOG_MP3(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new i = listitem;
	if(i == 0) {
		PlayRadio[playerid] = 0;
		StopAudioStreamForPlayer(playerid);
		RemovePlayerAttachedObject(playerid, 4);				
		return true;
	}
	if(i == sizeof(RadioLink)) Dialog_Show(playerid, DIALOG_YOUTUBE, DIALOG_STYLE_INPUT, "Youtube custom URL", "Scrie mai jos link-ul de la videoclipul de pe YouTube:", "Ok", "Exit");
	else {							
		PlayAudioStreamForPlayer(playerid, RadioLink[i]);	
		PlayRadio[playerid] = i;
		SetPlayerAttachedObject(playerid, 4, 19421, 2, 0.050000, -0.014999, 0.000000, 95.000000, 183.000000, 89.000000, 1, 1, 1, 0xffffffff);
	}	
	return 1;
}
Dialog:DIALOG_YOUTUBE(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(strfind(inputtext, "youtube", true) != -1) { }
	else return SCM(playerid, COLOR_GREY, "Invalid link!");			
	new string[144];
	format(string, sizeof(string), "http://www.youtubeinmp3.com/fetch/?video=%s", inputtext);
	PlayAudioStreamForPlayer(playerid, string);
	SetPlayerAttachedObject(playerid, 4, 19421, 2, 0.050000, -0.014999, 0.000000, 95.000000, 183.000000, 89.000000, 1, 1, 1, 0xffffffff);
	return 1;	
}	
Dialog:DIALOG_TICKET(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new giveplayerid = GetPVarInt(playerid, "userID");
	if(ProxDetectorS(8.0, playerid, giveplayerid)) { }
	else return SCM(playerid, -1, "Acel player nu este langa tine.");
	new giveplayer[64], sendername[MAX_PLAYER_NAME];
	new moneys = ticketValue(listitem);
	new result[64];
	format(result, 64, ticketName(listitem));
	GetPlayerName(giveplayerid, giveplayer, sizeof(giveplayer));
	GetPlayerName(playerid, sendername, sizeof(sendername));
	new string[144];
	format(string, sizeof(string), "* I-ai dat lui %s o amenda in valoare de $%s, motiv: %s", giveplayer, FormatNumber(moneys), result);
	SCM(playerid, COLOR_MONEY, string);
	format(string, sizeof(string), "* Ofiter %s ti-a dat o amenda in valoare de $%s, motiv: %s, scrie /accept ticket %d pentru a accepta.", sendername, FormatNumber(moneys), result,playerid);
	SCM(giveplayerid, COLOR_MONEY, string);
	TicketOffer[giveplayerid] = playerid;
	TicketMoney[giveplayerid] = moneys;	
	DeletePVar(playerid, "userID");			
	return 1;
}	
Dialog:DIALOG_TRADE(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(TradeReady[playerid] == 1 || TradeID[playerid] == -1) return true;
	switch(listitem) {
		case 0: {
			if(MoneyTrade[playerid] != 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Ai pus deja o suma de bani!");
			Dialog_Show(playerid, DIALOG_TRADE1, DIALOG_STYLE_INPUT, "Trade money", "Pune mai jos suma de bani pe care o doresti sa o pui!", "Ok", "Back");
		}
		case 1: {
			if(HouseTrade[playerid] != -1) return SCM(playerid, COLOR_LGREEN, "Eroare: Ai pus deja casa!");
			if(PlayerInfo[playerid][pHouse] != 999 && strcmp(GetName(playerid), HouseInfo[PlayerInfo[playerid][pHouse]][hOwner], true) == 0) { }
			else return SCM(playerid, COLOR_WHITE, "Ai deja o casa.");
			AddTradeItem(playerid, 1, 0);
		}
		case 2: {
			if(BusinessTrade[playerid] != -1) return SCM(playerid, COLOR_LGREEN, "Eroare: Ai pus deja afacerea!");
			if(PlayerInfo[playerid][pBizz] == 255) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu ai o afacere!");
			AddTradeItem(playerid, 2, 0);
		}		
		case 3: {
			if(CarTrade[playerid] != -1) 
				return SCM(playerid, COLOR_LGREEN, "Eroare: Ai pus deja un vehicul!");
			
			if(Iter_Count(MyVehicle[playerid]) == 0) 
				return SCM(playerid, COLOR_LGREEN, "Eroare: Nu ai un vehicul personal!");

			new szDialog[1000], x = 0;
			foreach(new idd: MyVehicle[playerid]) format(szDialog, sizeof(szDialog), "%s%s - %s\n", szDialog, aVehicleNames[CarInfo[idd][cModel]-400], TotalUp(idd)), PlayerInfo[playerid][pCarKey][x] = idd, x++;
			Dialog_Show(playerid, DIALOG_TRADE2, DIALOG_STYLE_LIST, "Trade vehicle", szDialog, "Ok", "Back");
		}
		case 4: {
			if(VoucherTrade[playerid][0] != 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Ai pus deja un voucher rare!");
			Dialog_Show(playerid, DIALOG_VOUCHERSS, DIALOG_STYLE_INPUT, "Trade Rare Voucher", "Pune mai jos suma de vouchere!", "Ok", "Back");
		}
		case 5: {
			if(VoucherTrade[playerid][1] != 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Ai pus deja un voucher epic!");
			Dialog_Show(playerid, DIALOG_VOUCHERSS1, DIALOG_STYLE_INPUT, "Trade Epic Voucher", "Pune mai jos suma de vouchere!", "Ok", "Back");
		}
		case 6: {
			if(VoucherTrade[playerid][2] != 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Ai pus deja un voucher legendar!");
			Dialog_Show(playerid, DIALOG_VOUCHERSS2, DIALOG_STYLE_INPUT, "Trade Legendary Voucher", "Pune mai jos suma de vouchere!", "Ok", "Back");
		}
		case 7: {
			if(SkinTrade[playerid] != -1) 
				return SCM(playerid, COLOR_LGREEN, "Eroare: Ai pus deja un skin!");

			if(GetSkinRare(GetPlayerSkin(playerid)) != 1)
				return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Trade Skin:", "Nu poti pune acest skin", "Ok", "");

			format(gString, sizeof(gString), "Esti sigur ca vrei sa pui acest skin %d (%s)?", GetPlayerSkin(playerid), GetSkinType(GetPlayerSkin(playerid)));
			Dialog_Show(playerid, DIALOG_TRADESKIN, DIALOG_STYLE_MSGBOX, "Trade Skin:", gString, "Select", "Cancel");
		}
		case 8: {
			if(HiddenTrade[playerid] != -1)
				return SCM(playerid, COLOR_LGREEN, "Eroare: Ai pus deja un hidden.");

			Dialog_Show(playerid, DIALOG_TRADEHIDDEN, DIALOG_STYLE_INPUT, "Trade Hidden:", "Insert amount hidden:", "Select", "Cancel");
		}
		case 9: {
			if(MiliardTrade[playerid] != 0)
				return SCM(playerid, COLOR_LGREEN, "Eroare: Ai pus deja un miliard(s)!");

			Dialog_Show(playerid, DIALOG_TRADEMILIARD, DIALOG_STYLE_INPUT, "Trade Miliard(s)", "Insert amount miliard(s):", "Select", "Cancel");
		}
		case 10: {
			if(TradeTicket[playerid][0] != 0) 
				return SCM(playerid, COLOR_LGREEN, "Eroare: Ai pus deja un rainbow tickets.");

			Dialog_Show(playerid, DIALOG_TRADEVOUCHERR, DIALOG_STYLE_INPUT, "Trade Rainbow Tickets:", "Introdu suma de tickete pe care vrei sa o pui:", "Select", "Cancel");
		}
		case 11: {
			if(TradeTicket[playerid][1] != 0) 
				return SCM(playerid, COLOR_LGREEN, "Eroare: Ai pus deja un vip tickets.");

			Dialog_Show(playerid, DIALOG_TRADEVOUCHERR, DIALOG_STYLE_INPUT, "Trade Vip Tickets:", "Introdu suma de tickete pe care vrei sa o pui:", "Select", "Cancel");
		}
	}
	return 1;
}
Dialog:DIALOG_TRADEMILIARD(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	new amount = strval(inputtext);

	if(amount < 1)
		return SCM(playerid, COLOR_LGREEN, "Eroare: Suma pe care ai introdus-o este invalida.");

	if(StoreMoney[playerid] < amount) 
		return SCM(playerid, COLOR_LGREEN, "Eroare: Value invalid.");

	AddTradeItem(playerid, 9, amount);
	return 1;
}
Dialog:DIALOG_TRADEVOUCHERR(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	if(strval(inputtext) <= 0)
		return SCM(playerid, COLOR_LGREEN, "Eroare: Suma invalida.");

	if(PlayerInfo[playerid][pTickete][0] < strval(inputtext))
		return SCM(playerid, COLOR_LGREEN, "Eroare: Nu ai aceste tickete.");

	AddTradeItem(playerid, 10, strval(inputtext));
	return 1;
}
Dialog:DIALOG_TRADEVOUCHERV(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	if(strval(inputtext) <= 0)
		return SCM(playerid, COLOR_LGREEN, "Eroare: Suma invalida.");

	if(PlayerInfo[playerid][pTickete][1] < strval(inputtext))
		return SCM(playerid, COLOR_LGREEN, "Eroare: Nu ai aceste tickete.");

	AddTradeItem(playerid, 11, strval(inputtext));
	return 1;
}
Dialog:DIALOG_TRADEHIDDEN(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	new amount = strval(inputtext);
	if(amount <= 0)
		return SCM(playerid, COLOR_GREY, "Invalid.");

	if(PlayerInfo[playerid][pHidden] < amount)
		return SCM(playerid, COLOR_LGREEN, "Eroare: Nu ai suficiente hiddenuri.");

	AddTradeItem(playerid, 8, amount);
	return 1;
}
Dialog:DIALOG_TRADESKIN(playerid, response, listitem, inputtext[]) {
	if(!response) return Dialog_Show(playerid, DIALOG_TRADE, DIALOG_STYLE_LIST, "Trade", "Money\nHouse\nBusiness\nVehicle\nRare Voucher (50 PP)\nEpic Voucher (100 PP)\nLegendary Voucher (500 PP)\nCurrent Skin\nHidden Color\nMoney (with miliards)\nTickets Rainbow\nTickets Vip", "Ok", "Cancel");
	AddTradeItem(playerid, 7, GetPlayerSkin(playerid));
	return 1;
}
Dialog:DIALOG_TRADE1(playerid, response, listitem, inputtext[]) {
	if(!response) return Dialog_Show(playerid, DIALOG_TRADE, DIALOG_STYLE_LIST, "Trade", "Money\nHouse\nBusiness\nVehicle\nRare Voucher (50 PP)\nEpic Voucher (100 PP)\nLegendary Voucher (500 PP)\nCurrent Skin\nHidden Color\nMoney (with miliards)\nTickets Rainbow\nTickets Vip", "Ok", "Cancel");
	if(strval(inputtext) < 1 || strval(inputtext) > 999999999) return SCM(playerid, COLOR_LGREEN, "Eroare: Poti pune in trade $1 si $999.999.999");
	if(PlayerMoney(playerid, strval(inputtext))) return SCM(playerid, COLOR_GREY, "Suma invalida!");
	AddTradeItem(playerid, 0, strval(inputtext));
	return 1;
}
Dialog:DIALOG_TRADE2(playerid, response, listitem, inputtext[]) {
	if(!response) return Dialog_Show(playerid, DIALOG_TRADE, DIALOG_STYLE_LIST, "Trade", "Money\nHouse\nBusiness\nVehicle\nRare Voucher (50 PP)\nEpic Voucher (100 PP)\nLegendary Voucher (500 PP)\nCurrent Skin\nHidden Color\nMoney (with miliards)\nTickets Rainbow\nTickets Vip", "Ok", "Cancel");
	new idd = PlayerInfo[playerid][pCarKey][listitem];
	if(CarInfo[idd][Spawned] == 0) 
		return SCM(playerid, COLOR_LGREEN, "Eroare: Acel vehicul nu este spawnat!");
	
	AddTradeItem(playerid, 3, idd);
	return 1;
}
Dialog:DIALOG_VOUCHERSS(playerid, response, listitem, inputtext[]) {
	if(!response) return Dialog_Show(playerid, DIALOG_TRADE, DIALOG_STYLE_LIST, "Trade", "Money\nHouse\nBusiness\nVehicle\nRare Voucher (50 PP)\nEpic Voucher (100 PP)\nLegendary Voucher (500 PP)\nCurrent Skin\nHidden Color\nMoney (with miliards)\nTickets Rainbow\nTickets Vip", "Ok", "Cancel");
	if(strval(inputtext) < 1) return SCM(playerid, COLOR_GREY, "Suma invalida!");
	if(PlayerInfo[playerid][pVoucher][0] < strval(inputtext)) return SCM(playerid, COLOR_GREY, "Suma invalida!");
	AddTradeItem(playerid, 4, strval(inputtext));
	return 1;
}
Dialog:DIALOG_VOUCHERSS1(playerid, response, listitem, inputtext[]) {
	if(!response) return Dialog_Show(playerid, DIALOG_TRADE, DIALOG_STYLE_LIST, "Trade", "Money\nHouse\nBusiness\nVehicle\nRare Voucher (50 PP)\nEpic Voucher (100 PP)\nLegendary Voucher (500 PP)\nCurrent Skin\nHidden Color\nMoney (with miliards)\nTickets Rainbow\nTickets Vip", "Ok", "Cancel");
	if(strval(inputtext) < 1) return SCM(playerid, COLOR_GREY, "Suma invalida!");
	if(PlayerInfo[playerid][pVoucher][1] < strval(inputtext)) return SCM(playerid, COLOR_GREY, "Suma invalida!");
	AddTradeItem(playerid, 5, strval(inputtext));
	return 1;
}
Dialog:DIALOG_VOUCHERSS2(playerid, response, listitem, inputtext[]) {
	if(!response) return Dialog_Show(playerid, DIALOG_TRADE, DIALOG_STYLE_LIST, "Trade", "Money\nHouse\nBusiness\nVehicle\nRare Voucher (50 PP)\nEpic Voucher (100 PP)\nLegendary Voucher (500 PP)\nCurrent Skin\nHidden Color\nMoney (with miliards)\nTickets Rainbow\nTickets Vip", "Ok", "Cancel");
	if(strval(inputtext) < 1) return SCM(playerid, COLOR_GREY, "Suma invalida!");
	if(PlayerInfo[playerid][pVoucher][2] < strval(inputtext)) return SCM(playerid, COLOR_GREY, "Suma invalida!");
	AddTradeItem(playerid, 6, strval(inputtext));
	return 1;
}
Dialog:DIALOG_DILDO(playerid, response, listitem, inputtext[]) { 
	if(!response) return true;
	switch(listitem) {
		case 0: {
			if(PlayerMoney(playerid, 500)) return SCM(playerid, COLOR_GREY, "Nu ai banii necesari pentru a cumpara acest lucru!");
			ServerWeapon(playerid, 10, 1);
			GivePlayerCash(playerid, 0, 500);
			SCM(playerid,COLOR_WHITE,"Ai cumparat Double-ended Dildo.");
			BizzInfo[6][bBalance] += 500;
		}
		case 1: {
			if(PlayerMoney(playerid, 500)) return SCM(playerid, COLOR_GREY, "Nu ai banii necesari pentru a cumpara acest lucru!");
			ServerWeapon(playerid, 11, 1);
			GivePlayerCash(playerid, 0, 500);
			SCM(playerid,COLOR_WHITE,"Ai cumparat Dildo.");
			BizzInfo[6][bBalance] += 500;
		}
		case 2: {
			if(PlayerMoney(playerid, 500)) return SCM(playerid, COLOR_GREY, "Nu ai banii necesari pentru a cumpara acest lucru!");
			ServerWeapon(playerid, 12, 1);
			GivePlayerCash(playerid, 0, 500);
			SCM(playerid,COLOR_WHITE,"Ai cumparat Vibrator.");
			BizzInfo[6][bBalance] += 500;
		}
		case 3: {
			if(PlayerMoney(playerid, 500)) return SCM(playerid, COLOR_GREY, "Nu ai banii necesari pentru a cumpara acest lucru!");
			ServerWeapon(playerid, 13, 1);
			GivePlayerCash(playerid, 0, 500);
			SCM(playerid,COLOR_WHITE,"Ai cumparat Silver Vibrator.");
			BizzInfo[6][bBalance] += 500;
		}
		case 4: {
			if(PlayerMoney(playerid, 1000)) return SCM(playerid, COLOR_GREY, "Nu ai banii necesari pentru a cumpara acest lucru!");
			ServerWeapon(playerid, 14, 1);
			GivePlayerCash(playerid, 0, 1000);
			SCM(playerid,COLOR_WHITE,"Ai cumparat flori.");
			BizzInfo[6][bBalance] += 1000;
		}
	}
	new string[144];
	mysql_format(SQL, string, sizeof(string), "UPDATE users SET Money='%d' WHERE `ID`='%s'", MoneyMoney[playerid], PlayerInfo[playerid][pSQLID]);
	mysql_tquery(SQL,string, "", "");
	mysql_format(SQL, string, sizeof(string), "UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='7'",BizzInfo[6][bBalance],BizzInfo[6][bProducts]);
	mysql_tquery(SQL,string, "", "");
	return 1;
}		
Dialog:DIALOG_STAFF(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new szQuery[1024], szResult[1024], szDialog[1024], szDialog2[4000], Cache: result;
	new szPlayers, szPlayersT;
	new szName[180], szAdmin, szLastOn[180], x, y, szTitle[180], warns, userID;
	switch(listitem) {
		case 0: {
			format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `Admin`>'0' ORDER BY `users`.`Admin` ASC LIMIT 50");
			result = mysql_query(SQL, szQuery);			
			strcat(szDialog2, "Name\tStatus\tRaport\tWarns\n");
			for(new i, j = cache_num_rows(); i < j; i++)
			{
				cache_get_value_name(i, "name", szResult); format(szName, 256, szResult);
				cache_get_value_name(i, "Admin", szResult); szAdmin = strval(szResult);
				cache_get_value_name(i, "lastOn", szResult); format(szLastOn, 256, "Offline (%s)", szResult);	
				cache_get_value_name(i, "HelpedPlayers", szResult); szPlayers = strval(szResult);
				cache_get_value_name(i, "HelpedPlayersToday", szResult); szPlayersT = strval(szResult);
				cache_get_value_name(i, "AW", szResult); warns = strval(szResult);
				userID = GetPlayerID( szName );
				userID = GetPlayerID( szName );
				if(userID != INVALID_PLAYER_ID) format(szLastOn, 256, "Online (%d)", userID);
				
				x++;
				format(szDialog, sizeof(szDialog), "(%d) %s\t%s\t%d on this week / %d total\t%d/3\n", szAdmin, szName, szLastOn, szPlayersT, szPlayers, warns);
				strcat(szDialog2, szDialog);			
			}
			cache_delete(result);
			format(szTitle, sizeof(szTitle), "{FFEE00}Admins (%d/%d)", GetStaffOnline(0), x);
		}
		case 1: {
			format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `Helper`>'0' ORDER BY `users`.`Helper` ASC LIMIT 50");
			result = mysql_query(SQL, szQuery);				
			strcat(szDialog2, "Name\tStatus\tRaport\tWarns\n");
			for(new i, j = cache_num_rows(); i < j; i++) {
				cache_get_value_name(i, "name", szResult); format(szName, 256, szResult);
				cache_get_value_name(i, "Helper", szResult); szAdmin = strval(szResult);
				cache_get_value_name(i, "lastOn", szResult); format(szLastOn, 256, "Offline (%s)", szResult);	
				cache_get_value_name(i, "HelpedPlayers", szResult); szPlayers = strval(szResult);
				cache_get_value_name(i, "HelpedPlayersToday", szResult); szPlayersT = strval(szResult);
				cache_get_value_name(i, "HW", szResult); warns = strval(szResult);
				
				userID = GetPlayerID( szName );
				if(userID != INVALID_PLAYER_ID) format(szLastOn, 256, "Online (%d)", userID);
				y++;
				format(szDialog, sizeof(szDialog), "(%d) %s\t%s\t%d today / %d total\t%d/3\n", szAdmin, szName, szLastOn, szPlayersT, szPlayers, warns);
				strcat(szDialog2, szDialog);		
			}
			cache_delete(result);
			format(szTitle, sizeof(szTitle), "{FFEE00}Helpers (%d/%d)", GetStaffOnline(1), y);
		}
	}
	Dialog_Show(playerid, DIALOG_STAFF1, DIALOG_STYLE_TABLIST_HEADERS, szTitle, szDialog2, "Ok", "Back");
	return 1;
}
Dialog:DIALOG_STAFF1(playerid, response, listitem, inputtext[]) {
	if(!response) return Dialog_Show(playerid, DIALOG_STAFF, DIALOG_STYLE_LIST, "Staff", "Admins\nHelpers", "Ok", "Close");
	return 1;
}
Dialog:DIALOG_REFERRALS(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new szQuery[256], szDialog[1024], Cache: result, money, rp, lvl, regdate[100], ore, name[100], szResult[256];
	
	if(listitem == 0)  {
		format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `id`='%d'", PlayerInfo[playerid][pReferral]);
		result = mysql_query(SQL, szQuery);
		cache_get_value_name(0, "name", szResult); format(name, 100, szResult);

		if(PlayerInfo[playerid][pReferral] != 0) format(szDialog, sizeof(szDialog),  "Ai fost adus pe comunitate de %s.\nTe-ai inregistrat pe data de %s\nCodul tau Referral este %d.", name, PlayerInfo[playerid][pRegistredDate], PlayerInfo[playerid][pSQLID]);
		else format(szDialog, sizeof(szDialog),  "Nu ai fost adus pe comunitate de cineva.\nTe-ai inregistrat pe %s\nCodul tau Referral este %d.\nAi adus in total %d jucatori.", PlayerInfo[playerid][pRegistredDate], PlayerInfo[playerid][pSQLID], CountReferrals(playerid));				
		Dialog_Show(playerid, DIALOG_REFERRALS1, DIALOG_STYLE_MSGBOX, "Referral info", szDialog, "Back", "Exit");			
		return true;
	}
	listitem --;
	format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `name`='%s'", Selected[playerid][listitem]);
	result = mysql_query(SQL, szQuery);
	cache_get_value_name(0, "RegisterDate", szResult); format(regdate, 100, szResult);
	cache_get_value_name(0, "Level", szResult); lvl = strval(szResult);
	cache_get_value_name(0, "ReferralRP", szResult); rp = strval(szResult);
	cache_get_value_name(0, "ReferralMoney", szResult); money = strval(szResult);
	cache_get_value_name(0, "ConnectedTime", szResult); ore = strval(szResult);			
	cache_delete(result);		
	format(szDialog, sizeof(szDialog), "Informatii despre %s\nInregistrat la data de: %s\nLevel: %d\nOre jucate: %d\nBani castigati de la el: $%s\nRP-uri castigate de la el: %d", Selected[playerid][listitem], regdate, lvl, ore, FormatNumber(money), rp);
	Dialog_Show(playerid, DIALOG_REFERRALS1, DIALOG_STYLE_MSGBOX, "Referral player", szDialog, "Back", "Exit");	
	return 1;
}
Dialog:DIALOG_REFERRALS1(playerid, response, listitem, inputtext[]) {
	return 1;
}
Dialog:DIALOG_SYSTEMS(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	switch(ServerSystem[listitem]) {
		case 0: {
			if(Licitatie == 1) SCMTA(COLOR_CLIENT, "SERVER: Sistemul de licitatii a fost dezactivat!");
			Licitatie = 0;
			TimpLicitatie = 0;		
			ServerSystem[listitem] = 1;
		}	
		case 1: ServerSystem[listitem] = 0;
	}
	new string[144];
	format(string, sizeof(string), "Warning: %s a %s sistemul '%s'.", GetName(playerid), (ServerSystem[listitem] == 0) ? ("activat") : ("dezactivat"), SystemName(listitem));
	SendAdminMessage(COLOR_WARNING, string, 5);	
	return 1;
}
Dialog:DIALOG_EMAILS(playerid, response, listitem, inputtext[]) {
	if(!response) return true;			
	switch(listitem) {
		case 0: Dialog_Show(playerid, DIALOG_SENDMAIL, DIALOG_STYLE_INPUT, "Check email", "Scrie adresa de email pe care vrei sa trimiti mesajul.", "Next", "Back");
		case 1: SetPVarInt(playerid, "Type", 0), ShowEmails(playerid, 0);
		case 2: SetPVarInt(playerid, "Type", 1), ShowEmails(playerid, 1);								
	}
	return 1;
}		
Dialog:DIALOG_EMAILS1(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new eid = EmailID[playerid][listitem], text[256];
	new qresult[5][256], query[144], string[300];				
	format(query, sizeof(query), "SELECT * FROM `emails` WHERE `ID`='%d'", eid);
	new Cache: result = mysql_query(SQL, query);	
	for(new i, x = cache_num_rows(); i < x; i++) {
		cache_get_value_name(i, "Text", string); format(qresult[0], 256, string);
		cache_get_value_name(i, "By", string); format(qresult[1], 256, string);
		cache_get_value_name(i, "Date", string); format(qresult[2], 256, string);	
		cache_get_value_name(i, "Read", string); format(qresult[3], 256, string);
		cache_get_value_name(i, "ID", string); format(qresult[4], 256, string);				
	}
	cache_delete(result);
	if(strval(qresult[3]) == 0) {
		mysql_format(SQL, query, sizeof(query), "UPDATE `emails` SET `Read`='1' WHERE `ID`='%d'", eid);
		mysql_tquery(SQL, query, "", "");
	}	
	if(GetPVarInt(playerid, "Type") == 0) format(text, sizeof(text), "%s\n\nEmail primit pe %s de la %s.", qresult[0], qresult[2], qresult[1]);
	else format(text, sizeof(text), "%s\n\nEmail trimis pe %s lui %s.", qresult[0], qresult[2], qresult[1]);
	Dialog_Show(playerid, DIALOG_EMAIL1, DIALOG_STYLE_MSGBOX, "Email", text, "Ok", "Back");		
	return 1;	
}	
Dialog:DIALOG_SENDMAIL(playerid, response, listitem, inputtext[]) {
	if(!response) return ShowEmails(playerid, 0);
	//if(strcmp(PlayerInfo[playerid][pEmail], inputtext, true) == 0) return Dialog_Show(playerid, DIALOG_SENDMAIL, DIALOG_STYLE_INPUT, "Check email", "Scrie adresa de email pe care vrei sa trimiti mesajul.", "Next", "Back");			
	new szQuery[256];
	new string[144];
	format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `Email`='%s' LIMIT 1", inputtext);
	new Cache: result = mysql_query(SQL, szQuery);
	new test = cache_num_rows();
	cache_delete(result);
	if(test == 0) return Dialog_Show(playerid, DIALOG_SENDMAIL, DIALOG_STYLE_INPUT, "Check email", "Email gresit!\nScrie adresa de email pe care vrei sa trimiti mesajul.", "Next", "Back");
	format(string, sizeof(string), "Scrie mesajul pe care doresti sa-l transmiti pe adresa %s\nNumarul minim de caractere este 10 iar cel maxim de 100.", inputtext);
	Dialog_Show(playerid, DIALOG_SENDMAIL1, DIALOG_STYLE_INPUT, "Send message", string, "Send", "Back");
	SetPVarString(playerid, "Email", inputtext);
	return 1;
}
Dialog:DIALOG_SENDMAIL1(playerid, response, listitem, inputtext[]) {
	if(!response) return Dialog_Show(playerid, DIALOG_SENDMAIL, DIALOG_STYLE_INPUT, "Check email", "Scrie adresa de email pe care vrei sa trimiti mesajul.", "Next", "Back");		
	new email[64];
	GetPVarString(playerid, "Email", email, sizeof(email));
	if(strlen(inputtext) < 10 || strlen(inputtext) > 100) {
		new string[144];
		format(string, sizeof(string), "Scrie mesajul pe care doresti sa-l transmiti pe adresa %s\nNumarul minim de caractere este 10 iar cel maxim de 100.", email);
		Dialog_Show(playerid, DIALOG_SENDMAIL1, DIALOG_STYLE_INPUT, "Send message", string, "Send", "Back");
		return true;
	}
	new string[144];	
	format(string, sizeof(string), "Email-ul a fost trimis cu succes!", email);
	SCM(playerid, COLOR_YELLOW, string);

	new name[64], query[144];
	format(query, sizeof(query), "SELECT * FROM `users` WHERE `Email`='%s'", email);
	new Cache: result = mysql_query(SQL, query);
	cache_get_value_name(0, "name", name); 
	cache_delete(result);					
	InsertEmail(name, GetName(playerid), inputtext, 1);			
	return 1;
}

Dialog:DIALOG_WHEELS(playerid, response, listitem, inputtext[]) {
	if(!response) 
		return true;
	
	new idd = PersonalCar(playerid);
	if(idd == -1) 
		return true;

	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	switch(listitem) {
		case 0: AddVehicleComponent(CarInfo[idd][Spawned], 1073);
		case 1: AddVehicleComponent(CarInfo[idd][Spawned], 1079);
		case 2: AddVehicleComponent(CarInfo[idd][Spawned], 1080);
		case 3: AddVehicleComponent(CarInfo[idd][Spawned], 1083);
	} 
	SCM(playerid, COLOR_YELLOW, "Modificari actualizate!");
	saveTuning(playerid, CarInfo[idd][Spawned]);
	BizzInfo[29][bBalance] += 50;				
	return 1;
}
Dialog:DIALOG_SPAWNCHANGE(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	switch(listitem) {
		case 0: {
			if(PlayerInfo[playerid][pHouse] == 999) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu ai o casa/chirie!");
			SCM(playerid, -1, "Vei fi spawnat la casa ta/chirie.");
			SpawnChange[playerid] = 1;	
			UpdateVar(playerid, "SpawnChange", 1);		
		}
		case 1: {
			SCM(playerid, -1, "Vei fi spawnat la locul normal.");
			SpawnChange[playerid] = 0;	
			UpdateVar(playerid, "SpawnChange", 0);		
		}
		case 2: {
			if(ClanInfo[PlayerInfo[playerid][pClan]][clHQ] == 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Clan-ul tau nu are un HQ cumparat!");
			SCM(playerid, -1, "Vei fi spawnat la HQ clanului.");
			SpawnChange[playerid] = 2;
			UpdateVar(playerid, "SpawnChange", 2);
		}		
	}
	
	return 1;
}
Dialog:DIALOG_BUYGUN2(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	switch(listitem) {
		case 0: ServerWeapon(playerid, 24, 50);		
		case 1: ServerWeapon(playerid, 31, 200);			
		case 2: ServerWeapon(playerid, 30, 200);	
		case 3: ServerWeapon(playerid, 28, 200);			
		case 4: ServerWeapon(playerid, 25, 20);					
	}
	Update(playerid, pCashx);
	return 1;
}
Dialog:DIALOG_SUSPECT(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new id = GetPVarInt(playerid, "Userid");
	new i = listitem;
	new string[144];	
	if(!IsPlayerConnected(id)) return SCM(playerid, COLOR_GREY, "Acel player nu mai este conectat!");
	new level = wantedList[i][0];
	if(level + PlayerInfo[id][pWantedLevel] > 6) PlayerInfo[id][pWantedLevel] = 6;
	else PlayerInfo[id][pWantedLevel] = level+PlayerInfo[id][pWantedLevel];
	format(string, sizeof(string), "Necunoscut");
	strmid(PlayerInfo[id][pVictim], string, 0, strlen(string), 255);
	Update(id,pVictimx);
	strmid(PlayerInfo[id][pAccused], GetName(playerid), 0, strlen(GetName(playerid)), 255);
	Update(id,pAccusedx);
	new y,m,d,h,mi;
	getdate(y,m,d);
	gettime(h,mi);
	if(strcmp(PlayerInfo[id][pCrime1], "Fara Crima", true) == 0) {
		format(string, sizeof(string), "%s - reporter: %s",wantedName(i),GetName(playerid));
		strmid(PlayerInfo[id][pCrime1], string, 0, strlen(string), 255);
		Update(id,pCrime1x);
	}
	else if(strcmp(PlayerInfo[id][pCrime2], "Fara Crima", true) == 0) {
		format(string, sizeof(string), "%s - reporter: %s",wantedName(i),GetName(playerid));
		strmid(PlayerInfo[id][pCrime2], string, 0, strlen(string), 255);
		Update(id,pCrime2x);
	}
	else if(strcmp(PlayerInfo[id][pCrime3], "Fara Crima", true) == 0) {
		format(string, sizeof(string), "%s - reporter: %s",wantedName(i),GetName(playerid));
		strmid(PlayerInfo[id][pCrime3], string, 0, strlen(string), 255);
		Update(id,pCrime3x);
	}
	else if(strcmp(PlayerInfo[id][pCrime1],  "Fara Crima", true) == 0 && strcmp(PlayerInfo[id][pCrime2], "Fara Crima", true) == 0 && strcmp(PlayerInfo[id][pCrime3], "Fara Crima", true) == 0) { }
	else
	{
		format(string, sizeof(string), "%s - reporter: %s",wantedName(i),GetName(playerid));
		strmid(PlayerInfo[id][pCrime1], string, 0, strlen(string), 255);
		Update(id,pCrime1x);
		format(string, sizeof(string), "Fara Crima");
		strmid(PlayerInfo[id][pCrime2], string, 0, strlen(string), 255);
		strmid(PlayerInfo[id][pCrime3], string, 0, strlen(string), 255);
		Update(id,pCrime2x);
		Update(id,pCrime3x);
	}
	SetPlayerWantedLevel(id,PlayerInfo[id][pWantedLevel]);
	format(string, sizeof(string), "Ai comis o infactiune: %s, raportat de: %s. W: +%d. Nivel urmarire nou: %d.",wantedName(i),GetName(playerid),level,PlayerInfo[id][pWantedLevel]);
	SCM(id, COLOR_WARNING, string);
	format(string, sizeof(string), "Dispatch: %s [%d] has commited a crime: %s. Reporter: %s. W: +%d. New wanted level: %d.",GetName(id),id,wantedName(i),GetName(playerid),level,PlayerInfo[id][pWantedLevel]);
	SendTeamMessage(14, 0x2e64feFF, string);
	SendTeamMessage(3, 0x2e64feFF, string);
	SendTeamMessage(2, 0x2e64feFF, string);
	SendTeamMessage(1, 0x2e64feFF, string);
	PlayCopSound();
	Update(id,pWantedLevelx);
	ShowWanted[id] = 1;	
	foreach(new x: Player) {
		if(IsACop(x) && OnDuty[x] == 1 && GetDistanceBetweenPlayers(x, playerid) < 250) {
			OnPlayerStreamOut(x, id);
			OnPlayerStreamIn(x, id);	
		}	
	}
	WantedTime[playerid] = 0;
	return 1;
}
Dialog:DIALOG_EDITSF(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	SelectedItem[playerid] = listitem+1;
	new string[144];
	format(string, sizeof(string), "Position\t(%f, %f, %f)\nRange\t%d", SafeZone[listitem+1][szX], SafeZone[listitem+1][szY], SafeZone[listitem+1][szZ], SafeZone[listitem+1][szRange]);
	new title[180];
	format(title, sizeof(title), "Edit safe zone %d", listitem+1);
	Dialog_Show(playerid, DIALOG_EDITSF1, DIALOG_STYLE_TABLIST, title, string, "Select", "Cancel");
	return 1;
}
Dialog:DIALOG_EDITSF1(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	switch(listitem) {
		case 0: Dialog_Show(playerid, DIALOG_EDITSF2, DIALOG_STYLE_INPUT, "Position:", "Scrie putin mai jos valoarea pe care vrei sa o aiba aceasta variabila.\nSyntax: x, y, z", "Ok", "Iesi");
		case 1: Dialog_Show(playerid, DIALOG_EDITSF2, DIALOG_STYLE_INPUT, "Range:", "Scrie putin mai jos valoarea pe care vrei sa o aiba aceasta variabila.", "Ok", "Iesi");
	}
	EditJob[playerid] = listitem;
	return 1;
}
Dialog:DIALOG_EDITSF2(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new job = SelectedItem[playerid];
	new amount = strval(inputtext);
	new string[144], query[144];
	switch(EditJob[playerid]) {
		case 0: {
			new Float: Pos[3];
			if(sscanf(inputtext, "fff", Pos[0], Pos[1], Pos[2])) return SCM(playerid, -1, "Invalid syntax!");
			SafeZone[job][szX] = Pos[0];
			SafeZone[job][szY] = Pos[1];
			SafeZone[job][szZ] = Pos[2];
			format(string, sizeof(string), "Ai modificat 'Pozitiile' la jobul cu ID-ul %d, in: %f, %f, %f.", job, Pos[0], Pos[1], Pos[2]);
			SCM(playerid, COLOR_WHITE, string);
			mysql_format(SQL, query, sizeof(query), "UPDATE `safezones` SET `X`='%f', `Y`='%f', `Z`='%f' WHERE `ID`='%d'", Pos[0], Pos[1], Pos[2], job);
			mysql_tquery(SQL, query, "", "");
		}
		case 1: {
			SafeZone[job][szRange] = amount;
			format(string, sizeof(string), "Ai modificat 'Raza' la safezone-ul cu ID-ul %d, in: %d.", job, amount);
			SCM(playerid, COLOR_WHITE, string);
			mysql_format(SQL, query, sizeof(query), "UPDATE `safezones` SET `Range`='%d' WHERE `ID`='%d'", amount, job);
			mysql_tquery(SQL, query, "", "");
		}
	}
	return 1;
}
Dialog:DIALOG_MYBL(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new i = SelectedFaction2[playerid][listitem];
	if(!IsInBlacklist(playerid, i)) return SCM(playerid, COLOR_GREY, "Nu esti pe blacklist-ul acelei factiuni! Probabil ai fost scos.");
	if(PlayerMoney(playerid, 100000)) return SCM(playerid, COLOR_GREY, "Nu ai $100,000 pentru a iesi de pe blacklist-ul acestei factiuni!");
	GivePlayerCash(playerid, 0, 100000);
	new string[144], query[144];
	mysql_format(SQL, query, sizeof(query), "DELETE FROM `blacklist` WHERE `Userid`='%d' AND `Faction`='%d'", PlayerInfo[playerid][pSQLID], i);
	mysql_tquery(SQL, query, "", "");
	format(string, sizeof(string), "* Ai platit $100,000 pentru a iesi de pe blacklist-ul factiunii %s!", FactionName(i));
	SCM(playerid, COLOR_MONEY, string);
	format(string, sizeof(string), "* %s (%d) a platit $100,000 pentru a iesi de pe blacklist-ul factiunii.", GetName(playerid), playerid);
	SendFactionMessage(i, 0x0080FFFF, string);
	SafeInfo[i-1][sMoney] += 100000;
	SaveSeif(i-1);	
	return 1;
}
Dialog:DIALOG_BMENU(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new biz = PlayerInfo[playerid][pBizz];
	new string[144], query[144];	
	switch(listitem) {
		case 0: {
			format(string, sizeof(string), "Nume: %s\nTaxa: $%s\nDepozit: $%s", BizzInfo[biz][bMessage], FormatNumber(BizzInfo[biz][bFee]), FormatNumber(BizzInfo[biz][bBalance]));
			Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Informatii", string, "Ok", "");
		}
		case 1: {
			format(string, sizeof(string), "In depozitul afacerii sunt $%s\nScrie mai jos suma pe care doresti s-o scoti!\nPentru a scoate o suma de bani, trebuie sa fii in interiorul afacerii!", FormatNumber(BizzInfo[biz][bBalance]));
			Dialog_Show(playerid, DIALOG_BMENU1, DIALOG_STYLE_INPUT, "Depozit", string, "Ok", "Inapoi");
		}
		case 2: {
			if(BizzInfo[biz][bLocked] == 1) {
				BizzInfo[biz][bLocked] = 0;
				SCM(playerid, COLOR_LGREEN, "Afacerea ta a fost deschisa!");
			}
			else if(BizzInfo[biz][bLocked] == 0) {
				BizzInfo[biz][bLocked] = 1;
				SCM(playerid, COLOR_LGREEN, "Afacerea ta a fost inchisa!");
			}
			mysql_format(SQL, query, sizeof(query),"UPDATE `bizz` SET `Locked`='%s' WHERE `ID`='%d'",BizzInfo[biz][bLocked], BizzInfo[biz][bID]);
			mysql_tquery(SQL,query, "", "");
		}
		case 3: Dialog_Show(playerid, DIALOG_BMENU1, DIALOG_STYLE_INPUT, "Seteaza numele", "Scrie mai jos numele pe care vrei sa-l aiba afacerea.", "Ok", "Inapoi");
		case 4: Dialog_Show(playerid, DIALOG_BMENU1, DIALOG_STYLE_INPUT, "Seteaza taxa", "Scrie mai jos taxa pe care vrei sa fie la intrarea in afacere.\n", "Ok", "Inapoi");
		case 5: {
			if(TradeID[playerid] != -1) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu poti folosi aceasta comanda atata timp cat esti implicat intr-o afacere!");
			Dialog_Show(playerid, DIALOG_BMENU1, DIALOG_STYLE_INPUT, "Vinde afacerea", "Scrie mai jos suma cu care vrei sa-ti vinzi afacerea.\nGandeste-te bine inainte de a face aceasta alegere.\nDaca vrei s-o scoti din vanzare, pune 0$ si se va scoate automat.", "Ok", "Inapoi");
		}
		case 6: {
			if(BizzInfo[biz][bRadio] == -1) return SCM(playerid, -1, "Nu ai un radio. Il poti cumpara din /shop cu 15 Premium Points.");
			new szDialog[1024], szDialog2[1024];
			strcat(szDialog2, "Nume\tAscultatori\n");
			for(new i = 0; i < sizeof(RadioNames); i++) {
				if(i == 0) format(szDialog, sizeof(szDialog), "%s\n", RadioNames[i]);
				else format(szDialog, sizeof(szDialog), "%s\t%d\n", RadioNames[i], ListenThisRadio(i));
				strcat(szDialog2, szDialog);
			}
			Dialog_Show(playerid, DIALOG_BRADIO, DIALOG_STYLE_TABLIST_HEADERS, "Business Radio", szDialog2, "Ok", "Close");		
		}
	}
	SelectedItem[playerid] = listitem;
	return 1;
}
Dialog:DIALOG_BMENU1(playerid, response, listitem, inputtext[]) {
	new biz = PlayerInfo[playerid][pBizz];
	new string[144], query[144];
	if(!response) {
		if(biz == 255) return SCM(playerid, COLOR_GREY, "Nu ai o afacere!");
		new open[64];
		if(biz < 100) {
			if(BizzInfo[biz][bLocked] == 0) open = "Inchide usa";
			else open = "Deschide usa";
		}
		else open = "Inchide usa";
		format(string, sizeof(string), "Informatii\nDepozit\n%s\nSeteaza numele\nSeteaza taxa\nVinde afacerea\nRadio", open);
		Dialog_Show(playerid, DIALOG_BMENU, DIALOG_STYLE_LIST, "Business menu", string, "Ok", "Exit");			
		return true;
	}
	new suma = strval(inputtext);
	switch(SelectedItem[playerid]) {
		case 1: {
			if(suma > BizzInfo[biz][bBalance] || suma < 1) return SCM(playerid, COLOR_GREY, "Nu ai aceasta suma!");
			if(!PlayerToPoint(50, playerid,BizzInfo[biz][bExitX],BizzInfo[biz][bExitY],BizzInfo[biz][bExitZ]) && BizzInfo[biz][bStatic] == 0) return SCM(playerid, COLOR_GREY, "Nu esti langa afacerea ta!");
			else if(!PlayerToPoint(5, playerid,BizzInfo[biz][bEntranceX],BizzInfo[biz][bEntranceY],BizzInfo[biz][bEntranceZ]) && BizzInfo[biz][bStatic] == 1) return SCM(playerid, COLOR_GREY, "Nu esti langa afacerea ta!");
			GivePlayerCash(playerid, 1,suma);
			BizzInfo[biz][bBalance] -= suma;
			format(string, sizeof(string), "Ai scos $%s din depozit. Acum mai sunt $%s.", FormatNumber(suma),FormatNumber(BizzInfo[biz][bBalance]));
			SCM(playerid, COLOR_MONEY, string);
			mysql_format(SQL, query, sizeof(query),"UPDATE `bizz` SET `Till`='%d' WHERE `ID`='%d'",BizzInfo[biz][bBalance], BizzInfo[biz][bID]);
			mysql_tquery(SQL,query, "", "");
			Update(playerid, pCashx);				
		}
		case 3: {
			if(strlen(inputtext) < 5 || strlen(inputtext) > 60) return SCM(playerid, COLOR_GREY, "Numele trebuie sa contina intre 5 si 60 de caractere!");
			strmid(BizzInfo[biz][bMessage], inputtext, 0, 64, 255);
			format(string, sizeof(string), "Numele afacerii a fost modificat in '%s'.",BizzInfo[biz][bMessage]);
			SCM(playerid, COLOR_LGREEN, string);
			mysql_format(SQL, query, sizeof(query),"UPDATE bizz SET Message='%s' WHERE ID='%d'",BizzInfo[biz][bMessage], BizzInfo[biz][bID]);
			mysql_tquery(SQL,query, "", "");
			UpdateLabel(2, biz);		
		}
		case 4: {
			if(suma > ValidBizPrice(biz) || suma < 1) {
				format(string, sizeof(string), "La afacerea ta poti pune maxim $%s ca taxa.", FormatNumber(ValidBizPrice(biz)));
				SCM(playerid, COLOR_GREY, string);
				return true;					
			}
			BizzInfo[biz][bFee] = suma;
			format(string, sizeof(string), "Taxa este acum de $%s.", FormatNumber(BizzInfo[biz][bFee]));
			SCM(playerid, COLOR_LGREEN, string);
			mysql_format(SQL, query, sizeof(query),"UPDATE `bizz` SET `EntranceCost`='%d' WHERE `ID`='%d'",BizzInfo[biz][bFee], BizzInfo[biz][bID]);
			mysql_tquery(SQL,query, "", "");
			UpdateLabel(2,biz);		
		}
		case 5: {
			if(suma < 0 || suma > 50000000) return SCM(playerid, COLOR_GREY, "Invalid suma! ($1-$50,000,000)");
			format(string, sizeof(string), "Ai setat pretul afacerii la $%s.", FormatNumber(suma));
			SCM(playerid, COLOR_MONEY, string);
			BizzInfo[biz][bBuyPrice]= suma;
			mysql_format(SQL, query, sizeof(query), "UPDATE `bizz` SET `BuyPrice`= '%d' WHERE `ID`= '%d'", suma, BizzInfo[biz][bID]);
			mysql_tquery(SQL,query, "", "");
			UpdateLabel(2,biz);
			format(string, sizeof(string), "%s si-a pus afacerea (%d) la vanzare pentru suma de $%s.", GetName(playerid), biz, FormatNumber(suma));
			InsertLog(playerid, string, LOG_SELL);				
		}
	}
	return 1;
}		
Dialog:DIALOG_HMENU(playerid, response, listitem, inputtext[]) {
	if(!response) return true;			
	if(PlayerInfo[playerid][pHouse] != 999 && strcmp(GetName(playerid), HouseInfo[PlayerInfo[playerid][pHouse]][hOwner], true) == 0) { }
	else return SCM(playerid, COLOR_GREY, "Nu ai o casa!");			
	new house = PlayerInfo[playerid][pHouse];
	new string[144], query[144];
	switch(listitem) {
		case 0: {
			format(string, sizeof(string), "Numele casei: %s\nBalanta: $%s\nChiriasi online: %d", HouseInfo[house][hDiscription], FormatNumber(HouseInfo[house][hTakings]), houseTenants(house));
			Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Informatii", string, "Ok", "");
		}
		case 1: showTenants(playerid);
		case 2: {
			format(string, sizeof(string), "In depozitul casei sunt $%s\nScrie mai jos suma pe care doresti s-o scoti!\nPentru a scoate o suma de bani, trebuie sa fii in interiorul casei!", FormatNumber(HouseInfo[house][hTakings]));
			Dialog_Show(playerid, DIALOG_HMENU1, DIALOG_STYLE_INPUT, "Depozit", string, "Ok", "Inapoi");
		}
		case 3: {
			if(!PlayerToPoint(5, playerid,HouseInfo[house][hEntrancex], HouseInfo[house][hEntrancey], HouseInfo[house][hEntrancez]) || PlayerToPoint(3, playerid,HouseInfo[house][hExitx], HouseInfo[house][hExity], HouseInfo[house][hExitz]))
				return SCM(playerid, COLOR_GREY, "Nu esti langa usa casei!");
				
			if(HouseInfo[house][hLock] == 1) {
				HouseInfo[house][hLock] = 0;
				SCM(playerid, COLOR_LGREEN, "Ai deschis usa casei!");
			}
			else if(HouseInfo[house][hLock] == 0) {
				HouseInfo[house][hLock] = 1;
				SCM(playerid, COLOR_LGREEN, "Ai inchis usa casei!");
			}	
			PlayerPlaySound(playerid, 1145, 0.0, 0.0, 0.0);
			mysql_format(SQL, query, sizeof(query),"UPDATE `houses` SET `Lockk`='%d' WHERE `ID`='%d'",HouseInfo[house][hLock], HouseInfo[house][hID]);
			mysql_tquery(SQL,query, "", "");
		}
		case 4: Dialog_Show(playerid, DIALOG_HMENU1, DIALOG_STYLE_LIST, "House upgrade", "Health Upgrade ($500)\nArmour Upgrade ($1000)", "Ok", "Inapoi");
		case 5: Dialog_Show(playerid, DIALOG_HMENU1, DIALOG_STYLE_INPUT, "Seteaza numele", "Scrie mai jos numele pe care vrei sa-l aiba casa.", "Ok", "Inapoi");
		case 6: Dialog_Show(playerid, DIALOG_HMENU3, DIALOG_STYLE_MSGBOX, "Chirie:", "Vrei sa ai chirie la aceasta casa?", "Yes", "No");
		case 7: {
			if(TradeID[playerid] != -1) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu poti folosi aceasta comanda atata timp cat esti implicat intr-o afacere!");
			Dialog_Show(playerid, DIALOG_HMENU1, DIALOG_STYLE_INPUT, "Vinde casa", "Scrie mai jos suma cu care vrei sa-ti vinzi casa.\nGandeste-te bine inainte de a face aceasta alegere.\nDaca vrei s-o scoti din vanzare, pune 0$ si se va scoate automat.", "Ok", "Inapoi"); 
		}
		case 8: {
			if(HouseInfo[house][hRadio] == -1) return SCM(playerid, -1, "Nu ai un radio. Il poti cumpara din /shop cu 15 Premium Points.");
			new szDialog[1024], szDialog2[1024];
			strcat(szDialog2, "Nume\tAscultatori\n");
			for(new i = 0; i < sizeof(RadioNames); i++) {
				if(i == 0) format(szDialog, sizeof(szDialog), "%s\n", RadioNames[i]);
				else format(szDialog, sizeof(szDialog), "%s\t%d\n", RadioNames[i], ListenThisRadio(i));
				strcat(szDialog2, szDialog);
			}
			Dialog_Show(playerid, DIALOG_HRADIO, DIALOG_STYLE_TABLIST_HEADERS, "House Radio", szDialog2, "Ok", "Close");		
		}
		case 9: Dialog_Show(playerid, DIALOG_HMENU1, DIALOG_STYLE_LIST, "House Upgrades", "Upgrade Garage", "Select", "Cancel");
	}
	SelectedItem[playerid] = listitem;
	return 1;
}	
Dialog:DIALOG_HMENU1(playerid, response, listitem, inputtext[]) {
	new string[144], query[144];
	if(!response) {
		if(PlayerInfo[playerid][pHouse] != 999 && strcmp(GetName(playerid), HouseInfo[PlayerInfo[playerid][pHouse]][hOwner], true) == 0) {
			new open[64];
			if(HouseInfo[PlayerInfo[playerid][pHouse]][hLock] == 0) open = "Inchide usa";
			else open = "Deschide usa";
			format(string, sizeof(string), "Informatii\nChiriasi\nDepozit\n%s\nHouse upgrade\nSeteaza numele\nSeteaza chiria\nVinde casa\nRadio\nHouse Upgrade", open);
			Dialog_Show(playerid, DIALOG_HMENU, DIALOG_STYLE_LIST, "House menu", string, "Ok", "Exit");
		}
		else SCM(playerid, COLOR_GREY, "Nu ai o casa!");			
		return true;
	}
	if(PlayerInfo[playerid][pHouse] != 999 && strcmp(GetName(playerid), HouseInfo[PlayerInfo[playerid][pHouse]][hOwner], true) == 0) { }
	else return SCM(playerid, COLOR_GREY, "Nu ai o casa!");
	new suma = strval(inputtext), house = PlayerInfo[playerid][pHouse];
	switch(SelectedItem[playerid]) {
		case 2: {
			if(suma > HouseInfo[house][hTakings] || suma < 1) return SCM(playerid, COLOR_GREY, "Nu ai aceasta suma de bani in depozitul casei!");
			if(!PlayerToPoint(100, playerid,HouseInfo[house][hExitx],HouseInfo[house][hExity],HouseInfo[house][hExitz])) return SCM(playerid, COLOR_GREY, "Nu esti in casa ta!");
			GivePlayerCash(playerid, 1, suma);
			HouseInfo[house][hTakings] -= suma;
			format(string, sizeof(string), "Ai scos $%s din depozit. Acum mai ai: $%s", FormatNumber(suma),FormatNumber(HouseInfo[house][hTakings]));
			SCM(playerid, COLOR_MONEY, string);
			Update(playerid, pCashx);
			mysql_format(SQL, query, sizeof(query),"UPDATE `houses` SET `Takings`='%d' WHERE `ID`='%d'",HouseInfo[house][hTakings],HouseInfo[house][hID]);
			mysql_tquery(SQL,query, "", "");
		}	
		case 4: {
			switch(listitem) {
				case 0:
				{
					if(PlayerMoney(playerid, 500)) return SCM(playerid, COLOR_GREY, "Nu ai destui bani!");
					if(HouseInfo[house][hHel] == 1) return SCM(playerid, COLOR_GREY, "Casa ta are deja acest item!");
					HouseInfo[house][hHel] = 1;
					GivePlayerCash(playerid, 0,500);
					
					BizzInfo[18][bBalance] += 500;
					mysql_format(SQL,query, sizeof(query),"UPDATE `bizz` SET `Till`='%d' WHERE `ID`='18'",BizzInfo[18][bBalance]);
					mysql_tquery(SQL,query, "", "");
			
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					mysql_format(SQL,query, sizeof(query),"UPDATE `houses` SET `Hel`='1' WHERE `ID`='%d'",HouseInfo[house][hID]);
					mysql_tquery(SQL,query, "", "");
				}
				case 1:
				{
					if(!IsACop(playerid)) return SCM(playerid, COLOR_GREY, "Nu esti politist!");
					if(PlayerMoney(playerid, 1000)) return SCM(playerid, COLOR_GREY, "Nu ai destui bani!");
					if(HouseInfo[house][hArm] == 1) return SCM(playerid, COLOR_GREY, "Casa ta are deja acest item!");
					HouseInfo[house][hArm] = 1;
					GivePlayerCash(playerid, 0,1000);
					BizzInfo[18][bBalance] += 1000;
					mysql_format(SQL, query, sizeof(query),"UPDATE `bizz` SET `Till`='%d' WHERE `ID`='18'",BizzInfo[18][bBalance]);
					mysql_tquery(SQL,query, "", "");
					PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
					mysql_format(SQL, query, sizeof(query),"UPDATE `houses` SET `Arm`='1' WHERE `ID`='%d'",HouseInfo[house][hID]);
					mysql_tquery(SQL,query, "", "");
				}
			}
			SCM(playerid, COLOR_LGREEN, "Casa a fost actualizata! Foloseste comanda '/heal' pentru a lua viata.");
		}
		case 5: {
			if(strlen(inputtext) < 5 || strlen(inputtext) > 60) return SCM(playerid, COLOR_GREY, "Numele trebuie sa contina intre 5 si 60 de caractere!");
			format(HouseInfo[house][hDiscription], 256, inputtext);
			format(string, sizeof(string), "Numele casei a fost modificat in '%s'.",HouseInfo[house][hDiscription]);
			SCM(playerid, COLOR_LGREEN, string);
			mysql_format(SQL, query, sizeof(query),"UPDATE houses SET Discription='%s' WHERE ID='%d'",HouseInfo[house][hDiscription],PlayerInfo[playerid][pHouse]);
			mysql_tquery(SQL,query, "", "");
			UpdateLabel(1, house);					
		}
		case 7: {
			if(suma < 0 || suma > 300000000) return SCM(playerid, COLOR_GREY, "Invalid price! ($1-$300,000,000)");
			format(string, sizeof(string), "Ai setat pretul casei la $%s.", FormatNumber(suma));
			SCM(playerid, COLOR_MONEY, string);
			HouseInfo[house][hValue] = suma;
			mysql_format(SQL, query, sizeof(query), "UPDATE `houses` SET `Value`= '%d' WHERE `ID`= '%d'", HouseInfo[house][hValue], HouseInfo[house][hID]);
			mysql_tquery(SQL, query, "", "");
			UpdateLabel(1,house);
			format(string, sizeof(string), "%s si-a pus casa (%d) la vanzare pentru suma de $%s.", GetName(playerid), house, FormatNumber(suma));
			InsertLog(playerid, string, LOG_SELL);				
		}
		case 9: {
			if(HouseInfo[house][hGarage] != 0) return SCM(playerid, COLOR_LGREEN, "Ai deja un garaj.");
			Dialog_Show(playerid, DIALOG_HMENU2, DIALOG_STYLE_TABLIST_HEADERS, "Garage Upgrade", "Metod\tPrice\nPremium Points\t{EEC135}1600{FFFFFF}\nMoney\t{28DC36}$2.000.000.000", "Contiune", "Cancel");
		}
	}
	return 1;
}
Dialog:DIALOG_HMENU2(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new house = PlayerInfo[playerid][pHouse];
	switch(listitem) {
		case 0: {
			if(PlayerInfo[playerid][pPremiumPoints] < 1600) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu ai aceasta suma de Premium Points!");
			if(!PlayerToPoint(30, playerid, HouseInfo[house][hEntrancex], HouseInfo[house][hEntrancey], HouseInfo[house][hEntrancez])) return SCM(playerid, COLOR_GREY, "Nu esti in zona casei.");
		}
		case 1: {
			if(StoreMoney[playerid] < 2) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu ai aceasta suma de bani!");
			if(!PlayerToPoint(30, playerid, HouseInfo[house][hEntrancex], HouseInfo[house][hEntrancey], HouseInfo[house][hEntrancez])) return SCM(playerid, COLOR_GREY, "Nu esti in zona casei.");
		}
	}
	SelectedItem[playerid] = listitem;
	SetTimerEx("PlaceGarage", 20000, 0, "i", playerid);
	SCM(playerid, COLOR_LGREEN, "System Garage: {FFFFFF}Ai la dispozitie 20 de secunde pentru a alege locul unde v-a fi garajul.");
	return 1;
}
Dialog:DIALOG_(playerid, response, listitem, inputtext[]) {
	new house = PlayerInfo[playerid][pHouse];
	HouseInfo[house][hRentabil] = response;
	new query[144];
	mysql_format(SQL, query, sizeof(query), "UPDATE `houses` SET `Rentabil`='%d' WHERE `ID`='%d'", HouseInfo[house][hRentabil], HouseInfo[house][hID]);
	mysql_tquery(SQL, query, "", "");
	UpdateLabel(1, house);
	return 1;
}
Dialog:DIALOG_WANTED(playerid, response, listitem, inputtext[]) {
	if(!response) return true;			
	new id = SelectedPlayers[playerid][listitem];
	if(PlayerInfo[id][pWantedLevel] == 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Acel player nu mai are wanted!");
	new string[144];
	ShowMDC(playerid, id);
	UsedFind[playerid] = 1;
	new Float:X,Float:Y,Float:Z;
	GetPlayerPos(id, X,Y,Z);
	SetPlayerCheckpointEx(playerid, X,Y,Z, 3.5);
	targetfind[playerid] = id;
	CP[playerid] = 53;
	format(string, sizeof(string), "* Punctul rosu a fost setat pe %s (%d) care se afla in zona %s.",GetName(id),id,GetPlayerZone(id));
	SCM(playerid,COLOR_YELLOW,string);

	return 1;
}
Dialog:DIALOG_SERVER(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	switch(listitem) {
		case 0: Dialog_Show(playerid, DIALOG_SERVER1, DIALOG_STYLE_MSGBOX, "Server menu", "Esti sigur ca vrei sa debanezi toti jucatorii?", "Da", "Nu");
		case 1: Dialog_Show(playerid, DIALOG_SERVER1, DIALOG_STYLE_MSGBOX, "Server menu", "Esti sigur ca vrei sa dai restart la payday?", "Da", "Nu");
		case 2: Dialog_Show(playerid, DIALOG_SERVER1, DIALOG_STYLE_MSGBOX, "Server menu", "Esti sigur ca vrei sa bagi serverul in mententanta?", "Da", "Nu");
		case 3: Dialog_Show(playerid, DIALOG_SERVER1, DIALOG_STYLE_INPUT, "Server menu", "Scrie parola care doresti s-o aiba serverul\nDaca doresti s-o scoti, pune '0'.", "Ok", "Iesi");
		case 4: Dialog_Show(playerid, DIALOG_SERVER1, DIALOG_STYLE_INPUT, "Server menu", "Scrie numele care doresti sa-l aiba serverul", "Ok", "Iesi");
		case 5: Dialog_Show(playerid, DIALOG_SERVER1, DIALOG_STYLE_MSGBOX, "Server menu", "Esti sigur ca vrei sa stergi casele celor inactivi?", "Da", "Nu");
		case 6: Dialog_Show(playerid, DIALOG_SERVER1, DIALOG_STYLE_MSGBOX, "Server menu", "Esti sigur ca vrei sa stergi afacerile celor inactivi?", "Da", "Nu");
		case 7: Dialog_Show(playerid, DIALOG_SERVER1, DIALOG_STYLE_MSGBOX, "Server menu", "Esti sigur ca vrei sa resetezi quest-ul?", "Da", "Nu");
	}
	SelectedItem[playerid] = listitem;
	return 1;
}
Dialog:DIALOG_SERVER1(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new string[144], query[144];	
	switch(SelectedItem[playerid]) {
		case 0: {
			mysql_format(SQL, query, sizeof(query),"UPDATE users SET `BTemp`='0', `BYear`='0', `BMonth`='0', `BDay`='0', `Banned`='0'");
			mysql_tquery(SQL,query, "", "");
			SCM(playerid, COLOR_YELLOW, "Ai debanat toti jucatorii!");
		}
		case 1: {
			RestartPayday = 1;
			SCM(playerid, COLOR_YELLOW, "Serverul se va restarta la payday-ul urmator!");
		}
		case 2: {
			foreach(new i: Player) {
				if(IsPlayerConnected(i) && IsPlayerLogged[i] == 1) {
					if(PlayerInfo[i][pAdmin] == 0) {
						SCM(i, COLOR_CLIENT, "Serverul a intrat in mentenanta! Va rugam sa reveniti peste cateva minute.");
						KickEx(i);
					}
					else SCM(i, COLOR_CLIENT, "Serverul a intrat in mentenanta! Nu ai primit kick deoarece esti admin.");
				}
			}
			SendRconCommand("hostname "SERVER_NAME" RPG - Mentenanta (0.3.7)");
			SendRconCommand("password "SERVER_NAME"2k19");
		}
		case 3: {
			format(string, sizeof(string), "password %s", inputtext);
			SendRconCommand(string);
			if(strlen(inputtext) == 0) format(string, sizeof(string), "Ai scos parola de la server!");
			else format(string, sizeof(string), "Ai pus parola '%s' la server!", inputtext);
			SCM(playerid, COLOR_YELLOW, string);
		}
		case 4: {
			format(string, sizeof(string), "hostname %s", inputtext);
			SendRconCommand(string);
			format(string, sizeof(string), "Ai pus numele '%s' la server!", inputtext);
			SCM(playerid, COLOR_YELLOW, string);
		}
		case 5: {
			new szResult[180], Cache: result, name[64], userid, x, hours;
			for(new i = 1; i < housess; i++) {
				if(strcmp(HouseInfo[i][hOwner], "The State", true) == 0) { }
				else {
					format(query, sizeof(query), "SELECT * FROM `users` WHERE `name`='%s'", HouseInfo[i][hOwner]);
					result = mysql_query(SQL, query);
					cache_get_value_name(0, "id", szResult); userid = strval(szResult);
					cache_get_value_name(0, "HoursMonth", szResult); hours = strval(szResult);
					cache_get_value_name(0, "name", szResult); format(name, 64, szResult);
					cache_delete(result);
					if(hours <= 5) {
						format(string, sizeof(string), "Ai pierdut casa #%d deoarece ai avut sub 5 ore jucate in aceasta luna.", i);
						InsertEmail(HouseInfo[i][hOwner], "AdmBot", string, 0);
						format(query, sizeof(query), "UPDATE `users` SET `House`='999' WHERE `ID`='%d'", userid);
						mysql_tquery(SQL, query, "", "");
						x++;
						HouseInfo[i][hOwned] = 1;
						HouseInfo[i][hValue] = 0;
						strmid(HouseInfo[i][hOwner], "The State", 0, strlen("The State"), 255);
						format(query, sizeof(query),"UPDATE `houses` SET `Hel`='0',`Arm`='0',`Lockk`='0',`Owned`='1',`Owner`='The State',`Value`='0' WHERE `ID`='%d'",BizzInfo[i][bID]);
						mysql_tquery(SQL, query, "", "");
						UpdateLabel(2, i);
					}
				}
			}
			if(x != 0) format(string, sizeof(string), "Au fost sterse %d case din baza de date!", x);
			else format(string, sizeof(string), "Nu au fost gasite case ce ar putea fi sterse!");
			SCM(playerid, COLOR_YELLOW, string);
		}
		case 6: {
			new szResult[180], Cache: result, name[64], userid, x, hours;
			for(new i = 1; i < bussines; i++) {
				if(strcmp(BizzInfo[i][bOwner], "The State", true) == 0) { }
				else {
					format(query, sizeof(query), "SELECT * FROM `users` WHERE `name`='%s'", BizzInfo[i][bOwner]);
					result = mysql_query(SQL, query);
					cache_get_value_name(0, "id", szResult); userid = strval(szResult);
					cache_get_value_name(0, "HoursMonth", szResult); hours = strval(szResult);
					cache_get_value_name(0, "name", szResult); format(name, 64, szResult);
					cache_delete(result);
					if(hours <= 5) {
						format(string, sizeof(string), "Ai pierdut afacerea #%d deoarece ai avut sub 5 ore jucate in aceasta luna.", i);
						InsertEmail(BizzInfo[i][bOwner], "AdmBot", string, 0);
						format(query, sizeof(query), "UPDATE `users` SET `Bizz`='255' WHERE `ID`='%d'", userid);
						mysql_tquery(SQL, query, "", "");
						x++;
						BizzInfo[i][bLocked] = 0;
						BizzInfo[i][bOwned] = 1;
						BizzInfo[i][bBuyPrice] = 0;
						strmid(BizzInfo[i][bOwner], "The State", 0, strlen("The State"), 255);
						format(query, sizeof(query),"UPDATE `bizz` SET `Locked`='0',`Owned`='1',`Owner`='The State',`BuyPrice`='0' WHERE `ID`='%d'",BizzInfo[i][bID]);
						mysql_tquery(SQL, query, "", "");
						UpdateLabel(2, i);
					}
				}
			}
			if(x != 0) format(string, sizeof(string), "Au fost sterse %d afaceri din baza de date!", x);
			else format(string, sizeof(string), "Nu au fost gasite afaceri ce ar putea fi sterse!");
			SCM(playerid, COLOR_YELLOW, string);
		}
		case 7: {
			SCM(playerid, COLOR_YELLOW, "Quest-ul a fost resetat!");
			ResetQuest();
		}
	}
	return 1;
}
Dialog:DIALOG_ADDCOMMAND(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(ExistaComanda(inputtext)) return SCM(playerid, COLOR_GREY, "Aceasta comanda este deja dezactivata!");
	new reason[180], query[144];
	GetPVarString(playerid, "Reason", reason, sizeof(reason));
	mysql_format(SQL, query, sizeof(query), "INSERT INTO `commands` (`Name`, `Reason`) VALUES ('%s', '%s')", inputtext, reason);
	mysql_tquery(SQL, query, "", "");
	new string[144];	
	format(string, sizeof(string), "Comanda '%s' a fost dezactivata cu succes. Motivul: %s", inputtext, reason);
	SCM(playerid, COLOR_YELLOW, string);
	return 1;
}
Dialog:DIALOG_BUYGUN(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new query[144];
	switch(listitem) {
		case 0: {
			if(PlayerMoney(playerid, 2000)) return SCM(playerid, COLOR_GREY, "Ai nevoie de $2,000 pentru a cumpara aceasta arma!");
			ServerWeapon(playerid, 24, 50);
			SCM(playerid, COLOR_MONEY, "Ti-ai cumparat arma Deagle pentru suma de $2,000.");
			GivePlayerCash(playerid, 0, 2000);
			BizzInfo[InBussines[playerid]][bBalance] += 2000;
			mysql_format(SQL, query, sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL, query, "", "");
		}
		case 1: {
			if(PlayerMoney(playerid, 5000)) return SCM(playerid, COLOR_GREY, "Ai nevoie de $5,000 pentru a cumpara aceasta arma!");
			ServerWeapon(playerid, 31, 200);
			SCM(playerid, COLOR_MONEY, "Ti-ai cumparat arma M4 pentru suma de $5,000.");
			GivePlayerCash(playerid, 0, 5000);
			BizzInfo[InBussines[playerid]][bBalance] += 5000;
			mysql_format(SQL,query, sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL, query, "", "");
		}
		case 2: {
			if(PlayerMoney(playerid, 4800)) return SCM(playerid, COLOR_GREY, "Ai nevoie de $4,800 pentru a cumpara aceasta arma!");
			ServerWeapon(playerid, 30, 200);
			SCM(playerid, COLOR_MONEY, "Ti-ai cumparat arma AK47 pentru suma de $4,800.");
			GivePlayerCash(playerid, 0, 4800);
			BizzInfo[InBussines[playerid]][bBalance] += 4800;
			mysql_format(SQL,query, sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL, query, "", "");
		}	
		case 3: {
			if(PlayerMoney(playerid, 2500)) return SCM(playerid, COLOR_GREY, "Ai nevoie de $2,500 pentru a cumpara aceasta arma!");
			ServerWeapon(playerid, 28, 200);
			SCM(playerid, COLOR_MONEY, "Ti-ai cumparat arma Micro SMG pentru suma de $2,500.");
			GivePlayerCash(playerid, 0, 2500);
			BizzInfo[InBussines[playerid]][bBalance] += 2500;
			mysql_format(SQL,query, sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL, query, "", "");
		}	
		case 4: {
			if(PlayerMoney(playerid, 3000)) return SCM(playerid, COLOR_GREY, "Ai nevoie de $3,000 pentru a cumpara aceasta arma!");
			ServerWeapon(playerid, 25, 20);
			SCM(playerid, COLOR_MONEY, "Ti-ai cumparat arma Shotgun pentru suma de $3,000.");
			GivePlayerCash(playerid, 0, 3000);
			BizzInfo[InBussines[playerid]][bBalance] += 3000;
			mysql_format(SQL, query, sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL, query, "", "");
		}		
		case 5: {
			if(PlayerMoney(playerid, 3000)) return SCM(playerid, COLOR_GREY, "Ai nevoie de $3,000 pentru a cumpara aceasta arma!");
			ServerWeapon(playerid, 5, 1);
			SCM(playerid, COLOR_MONEY, "Ti-ai cumparat o bata pentru suma de $1,000.");
			GivePlayerCash(playerid, 0, 1000);
			BizzInfo[InBussines[playerid]][bBalance] += 1000;
			mysql_format(SQL, query, sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL, query, "", "");
		}
	}
	Update(playerid, pCashx);
	return 1;
}
Dialog:DIALOG_FDEPOSIT(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	switch(listitem) {
		case 0: {
			Dialog_Show(playerid, DIALOG_FDEPOSIT1, DIALOG_STYLE_INPUT, "Deposit", "Scrie suma de bani pe care vrei sa o depozitezi putin mai jos!", "Select", "Inapoi");
		}
		case 1: {
			Dialog_Show(playerid, DIALOG_FDEPOSIT1, DIALOG_STYLE_INPUT, "Deposit", "Scrie suma de materiale pe care vrei sa o depozitezi putin mai jos!", "Select", "Inapoi");
		}
		case 2: {
			Dialog_Show(playerid, DIALOG_FDEPOSIT1, DIALOG_STYLE_INPUT, "Deposit", "Scrie suma de droguri pe care vrei sa o depozitezi putin mai jos!", "Select", "Inapoi");
		}
	}
	SelectedItem[playerid] = listitem;
	return 1;
}
Dialog:DIALOG_FDEPOSIT1(playerid, response, listitem, inputtext[]) {
	if(!response) {
		Dialog_Show(playerid, DIALOG_FDEPOSIT, DIALOG_STYLE_LIST, "Faction deposit", "Money\nMaterials\nDrugs", "Ok", "Exit");
		return true;
	}
	new string[144];	
	switch(SelectedItem[playerid]) {
		case 0: {
			if(PlayerMoney(playerid, strval(inputtext)) || strval(inputtext) <= 0) return SCM(playerid, COLOR_WHITE, "* Suma invalida!");
			SafeInfo[SeifID[playerid]][sMoney] += strval(inputtext);
			GivePlayerCash(playerid, 0, strval(inputtext));
			format(string, sizeof(string), "* %s a depozitat $%s in seiful factiunii.", GetName(playerid), FormatNumber(strval(inputtext)));
			SendFactionMessage(PlayerInfo[playerid][pMember], COLOR_LIGHTBLUE, string);	
		}
		case 1: {
			if(PlayerInfo[playerid][pMats] < strval(inputtext) || strval(inputtext) <= 0) return SCM(playerid, COLOR_WHITE, "* Suma invalida!");
			SafeInfo[SeifID[playerid]][sMaterials] += strval(inputtext);
			PlayerInfo[playerid][pMats] -= strval(inputtext);
			format(string, sizeof(string), "* %s a depozitat %s materiale in seiful factiunii.", GetName(playerid), FormatNumber(strval(inputtext)));
			SendFactionMessage(PlayerInfo[playerid][pMember], COLOR_LIGHTBLUE, string);	
			UpdateVar(playerid, "Materials", PlayerInfo[playerid][pMats]);
		}
		case 2: {
			if(PlayerInfo[playerid][pDrugs] < strval(inputtext) || strval(inputtext) <= 0) return SCM(playerid, COLOR_WHITE, "* Suma invalida!");
			SafeInfo[SeifID[playerid]][sDrugs] += strval(inputtext);
			PlayerInfo[playerid][pDrugs] -= strval(inputtext);
			UpdateVar(playerid, "Drugs", PlayerInfo[playerid][pDrugs]);
			format(string, sizeof(string), "* %s a depozitat %s droguri in seiful factiunii.", GetName(playerid), FormatNumber(strval(inputtext)));
			SendFactionMessage(PlayerInfo[playerid][pMember], COLOR_LIGHTBLUE, string);		
		}			
	}
	SaveSeif(SeifID[playerid]);	
	return 1;
}		
Dialog:DIALOG_FWITHDRAW(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new string[144];
	switch(listitem) {
		case 0: {
			format(string, sizeof(string), "Seiful factiunii are in total: $%s.\nScrie suma pe care vrei sa o scoti putin mai jos!", FormatNumber(SafeInfo[SeifID[playerid]][sMoney]));
			Dialog_Show(playerid, DIALOG_FWITHDRAW1, DIALOG_STYLE_INPUT, "Withdraw", string, "Select", "Inapoi");
		}
		case 1: {
			format(string, sizeof(string), "Seiful factiunii are in total: %s materiale.\nScrie suma pe care vrei sa o scoti putin mai jos!", FormatNumber(SafeInfo[SeifID[playerid]][sMaterials]));
			Dialog_Show(playerid, DIALOG_FWITHDRAW1, DIALOG_STYLE_INPUT, "Withdraw", string, "Select", "Inapoi");
		}
		case 2: {
			format(string, sizeof(string), "Seiful factiunii are in total: %s droguri.\nScrie suma pe care vrei sa o scoti putin mai jos!", FormatNumber(SafeInfo[SeifID[playerid]][sDrugs]));
			Dialog_Show(playerid, DIALOG_FWITHDRAW1, DIALOG_STYLE_INPUT, "Withdraw", string, "Select", "Inapoi");
		}
	}
	SelectedItem[playerid] = listitem;
	return 1;
}
Dialog:DIALOG_FWITHDRAW1(playerid, response, listitem, inputtext[]) {
	new string[144];
	if(!response) {
		new s = SeifID[playerid];
		format(string, sizeof(string), "Option\tResult\nMoney\t$%s\nMaterials\t%s\nDrugs\t%s\n", FormatNumber(SafeInfo[s][sMoney]), FormatNumber(SafeInfo[s][sMaterials]), FormatNumber(SafeInfo[s][sDrugs]));
		Dialog_Show(playerid, DIALOG_FWITHDRAW, DIALOG_STYLE_TABLIST_HEADERS, "Faction withdraw", string, "Ok", "Exit");
		return true;
	}
	switch(SelectedItem[playerid]) {
		case 0: {
			if(SafeInfo[SeifID[playerid]][sMoney] < strval(inputtext) || strval(inputtext) <= 0) return SCM(playerid, COLOR_WHITE, "* Suma invalida!");
			SafeInfo[SeifID[playerid]][sMoney] -= strval(inputtext);
			GivePlayerCash(playerid, 1, strval(inputtext));
			format(string, sizeof(string), "Ai luat din seiful factiunii $%s! Acum in seif mai sunt $%s.", FormatNumber(strval(inputtext)), FormatNumber(SafeInfo[SeifID[playerid]][sMoney]));
			SCM(playerid, COLOR_MONEY, string);
		}
		case 1: {
			if(SafeInfo[SeifID[playerid]][sMaterials] < strval(inputtext) || strval(inputtext) <= 0) return SCM(playerid, COLOR_WHITE, "* Suma invalida!");
			SafeInfo[SeifID[playerid]][sMaterials] -= strval(inputtext);
			PlayerInfo[playerid][pMats] += strval(inputtext);
			format(string, sizeof(string), "Ai luat din seiful factiunii %s materiale! Acum in seif mai sunt %s materiale.", FormatNumber(strval(inputtext)), FormatNumber(SafeInfo[SeifID[playerid]][sMaterials]));
			SCM(playerid, COLOR_MONEY, string);
		}
		case 2: {
			if(SafeInfo[SeifID[playerid]][sDrugs] < strval(inputtext) || strval(inputtext) <= 0) return SCM(playerid, COLOR_WHITE, "* Suma invalida!");
			SafeInfo[SeifID[playerid]][sDrugs] -= strval(inputtext);
			PlayerInfo[playerid][pDrugs] += strval(inputtext);
			format(string, sizeof(string), "Ai luat din seiful factiunii $%s droguri! Acum in seif mai %s droguri.", FormatNumber(strval(inputtext)), FormatNumber(SafeInfo[SeifID[playerid]][sDrugs]));
			SCM(playerid, COLOR_MONEY, string);
		}
	}
	SaveSeif(SeifID[playerid]);	
	return 1;
}	
Dialog:DIALOG_QUITCLAN(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new query[250];
	ClanMembers[PlayerInfo[playerid][pClan]] --;
	PlayerInfo[playerid][pClan] = 0;
	PlayerInfo[playerid][pClanRank] = 0;
	PlayerInfo[playerid][pClanDays] = 0;
	PlayerInfo[playerid][pClanWarn] = 0;
	SetPlayerName(playerid, PlayerInfo[playerid][pUsername]);
	mysql_format(SQL, query, sizeof(query), "UPDATE `users` SET `Clan`='%d', `ClanRank`='%d', `ClanDays`='0', `ClanWarns`='0' WHERE `Name`='%s'", PlayerInfo[playerid][pClan], PlayerInfo[playerid][pClanRank], PlayerInfo[playerid][pUsername]);
	mysql_tquery(SQL, query, "", "");
	SCM(playerid, COLOR_YELLOW, "* Ai iesit din clan.");
	return 1;
}
Dialog:DIALOG_SELLBIZ(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(strcmp(GetName(playerid), BizzInfo[PlayerInfo[playerid][pBizz]][bOwner], true) == 0)
	{
		new string[144];		
		new bouse = PlayerInfo[playerid][pBizz];
		BizzInfo[bouse][bBuyPrice] = 0;
		BizzInfo[bouse][bLocked] = 1;
		BizzInfo[bouse][bOwned] = 1;
		strmid(BizzInfo[bouse][bOwner], "The State", 0, strlen("The State"), 255);
		new cashb = 150000;
		GivePlayerCash(playerid, 1,cashb);
		PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
		format(string, sizeof(string), "* Ti-ai vandut afacerea pentru suma de $%s.", FormatNumber(cashb));
		SCM(playerid, COLOR_MONEY, string);
		
		format(string, sizeof(string), "%s si-a vandut biz-ul (%d) la stat si a primit $150,000.", GetName(playerid), bouse);
		InsertLog(playerid, string, LOG_SELL);	
	
		PlayerInfo[playerid][pBizz] = 255;
		new str1[184];
		mysql_format(SQL, str1,sizeof(str1),"UPDATE `bizz` SET `Locked`='0',`Owned`='1',`Till`='0',`BuyPrice`='0',`Owner`='The State' WHERE `ID`='%d'",bouse);
		mysql_tquery(SQL,str1, "", "");
		Update(playerid,pCashx);
		Update(playerid,pBizzx);
		UpdateLabel(2,bouse);
	}
	else return SCM(playerid,COLOR_GREY, "Nu detii o afacere.");
	return 1;
}
Dialog:DIALOG_SELLHOUSE(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new house = PlayerInfo[playerid][pHouse];
	HouseInfo[house][hHel] = 0;
	HouseInfo[house][hArm] = 0;
	HouseInfo[house][hLock] = 0;
	HouseInfo[house][hOwned] = 1;
	HouseInfo[house][hValue] = 0;
	PlayerInfo[playerid][pRented] = 0;
	strmid(HouseInfo[house][hOwner], "The State", 0, strlen("The State"), 255);
	new str2[180];
	if(HouseInfo[house][hGarage] != 0) {
		strmid(GarageInfo[HouseInfo[house][hGarage]][gOwner], "The State", 0, strlen("The State"), 255), UpdateLabel(3, HouseInfo[house][hGarage]);
		mysql_format(SQL, str2,sizeof(str2),"UPDATE `garages` SET `Owner`='The State' WHERE `ID`='%d'",HouseInfo[house][hGarage]);
		mysql_tquery(SQL,str2, "", "");			
	}
	new string[144];	
	ConsumingMoney[playerid] = 1;
	GivePlayerCash(playerid, 1,50000);
	PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
	format(string, sizeof(string), "* Ti-ai vandut casa pentru suma de $50,000.");
	SCM(playerid, COLOR_MONEY, string);
	format(string, sizeof(string), "%s si-a vandut casa (%d) la stat si a primit $50,000.", GetName(playerid), house);
	InsertLog(playerid, string, LOG_SELL);			
	PlayerInfo[playerid][pHouse] = 999;
	PlayerInfo[playerid][pRented] = -1;
	if(InHouse[playerid] == house)
	{
		SetPlayerInterior(playerid,0);
		SetPlayerPosEx(false, playerid,HouseInfo[house][hEntrancex],HouseInfo[house][hEntrancey],HouseInfo[house][hEntrancez]);
		PlayerInfo[playerid][pInt] = 0;
	}
	Update(playerid,pCashx);
	Update(playerid,pRentedx);
	Update(playerid,pHousex);
	mysql_format(SQL, str2,sizeof(str2),"UPDATE `houses` SET `Hel`='0',`Arm`='0',`Lockk`='0',`Owned`='1',`Value`='0',`Owner`='The State' WHERE `ID`='%d'",HouseInfo[house][hID]);
	mysql_tquery(SQL,str2, "", "");
	UpdateLabel(1,house);
	return 1;
}
Dialog:DIALOG_CHECK(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new name[180], szQuery[256], szResult[180], check[180], string[144];
	GetPVarString(playerid, "CheckName", name, 256);
	format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `name`='%s'", name);
	new Cache: result = mysql_query(SQL, szQuery);
	switch(listitem) {
		case 0: cache_get_value_name(0, "Pin", szResult), format(check, 256, szResult);
		case 1: cache_get_value_name(0, "Money", szResult), format(check, 256, szResult);
		case 2: cache_get_value_name(0, "Bank", szResult), format(check, 256, szResult);
		case 3: cache_get_value_name(0, "House", szResult), format(check, 256, szResult);
		case 4: cache_get_value_name(0, "Bizz", szResult), format(check, 256, szResult);
		case 5: cache_get_value_name(0, "Level", szResult), format(check, 256, szResult);
		case 6: cache_get_value_name(0, "ConnectedTime", szResult), format(check, 256, szResult);
		case 7: cache_get_value_name(0, "GoldPoints", szResult), format(check, 256, szResult);
		case 8: cache_get_value_name(0, "GoldCastigat", szResult), format(check, 256, szResult);
		case 9: cache_get_value_name(0, "HoursMonth", szResult), format(check, 256, szResult);
		case 10: cache_get_value_name(0, "DayHours", szResult), format(check, 256, szResult);
		case 11: cache_get_value_name(0, "lastOn", szResult), format(check, 256, szResult);
		case 12: cache_get_value_name(0, "Member", szResult), format(check, 256, szResult);								
	}
	if(listitem != 13) format(string, sizeof(string), "* Database result: %s", check);
	else format(string, sizeof(string), "* Database result: %s", FactionName(strval(check)));
	SCM(playerid, 0xFFB657FFF, string);		
	cache_delete(result);	
	return 1;
}	
Dialog:DIALOG_BLACKLIST(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(listitem == 0) {
		Dialog_Show(playerid, DIALOG_BLACKLIST1, DIALOG_STYLE_INPUT, "Blacklist:", "Scrie mai jos ID-ul sau numele jucatorului pe care vrei sa-l adaugi.\nAcela trebuie sa fie online pentru a-l putea adauga.", "Ok", "Exit");
		return true;
	}
	format(ssName[playerid], 256, Selected[playerid][listitem-1]);
	new szTitle[180];
	format(szTitle, sizeof(szTitle), "%s", ssName[playerid]);
	Dialog_Show(playerid, DIALOG_BLACKLIST2, DIALOG_STYLE_LIST, szTitle, "{FF0000}Sterge de pe blacklist", "Select", "Back");
	return 1;
}
Dialog:DIALOG_BLACKLIST1(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new id;
	if(sscanf(inputtext, "u", id)) {
		if(!IsPlayerConnected(id)) return SCM(playerid, COLOR_WHITE, "Acel player nu este online!");
	}
	else {
		if(id == playerid) return SCM(playerid, COLOR_GREY, "Nu te poti adauga pe tine in lista!");
		if(PlayerInfo[id][pMember] == PlayerInfo[playerid][pMember]) return SCM(playerid, -1, "Nu poti adauga un membru in lista de blacklist!");
		if(!IsPlayerConnected(id)) return SCM(playerid, COLOR_WHITE, "Acel player nu este online!");
		if(IsInBlacklist(id, PlayerInfo[playerid][pLeader])) return SCM(playerid, -1, "Acel player este deja pe blacklist-ul acestei factiuni!");
		new string[144];
		format(string, sizeof(string), "%s (%d) face acum parte din lista de blacklist a factiunii.", GetName(id), id);
		SCM(playerid, COLOR_GOLD, string);
		format(string, sizeof(string), "%s (%d) te-a adaugat pe blacklist-ul factiunii %s.", GetName(playerid), playerid, FactionName(PlayerInfo[playerid][pLeader]));
		SCM(id, COLOR_GOLD, string);
		mysql_format(SQL, string, sizeof(string), "INSERT INTO `blacklist` (`Userid`, `Name`, `Faction`, `Added`) VALUES ('%d', '%s', '%d', '%s')", PlayerInfo[id][pSQLID], PlayerInfo[id][pUsername], PlayerInfo[playerid][pLeader], PlayerInfo[playerid][pUsername]);
		mysql_tquery(SQL, string, "", "");
	}
	return 1;
}
Dialog:DIALOG_BLACKLIST2(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new string[144];
	format(string, sizeof(string), "%s nu mai face parte din lista de blacklist.", ssName[playerid]);
	SCM(playerid, COLOR_GOLD, string);
	new userID = GetPlayerID( ssName[playerid] );
	if(userID != INVALID_PLAYER_ID) {
		mysql_format(SQL, string,128,"DELETE FROM `blacklist` WHERE `Userid`='%d'",PlayerInfo[userID][pSQLID]);
		mysql_query(SQL, string);
		format(string, sizeof(string), "%s te-a scos de pe lista de blacklist a factiunii %s.", GetName(playerid), FactionName(PlayerInfo[playerid][pLeader]));
		SCM(userID, COLOR_GOLD, string);
	}
	else {
		mysql_format(SQL, string,128,"DELETE FROM `blacklist` WHERE `Name`='%s'",ssName[playerid]);
		mysql_query(SQL, string);
	}
	return 1;
}		
Dialog:DIALOG_FRIENDS(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(listitem == 0) {
		Dialog_Show(playerid, DIALOG_FRIENDS1, DIALOG_STYLE_INPUT, "Adaugare prieten:", "Scrie mai jos ID-ul sau numele jucatorului pe care vrei sa-l adaugi.\nAcela trebuie sa fie online pentru a-l putea adauga.", "Ok", "Exit");
		return true;
	}
	format(ssName[playerid], 256, Selected[playerid][listitem-1]);
	new userid = GetPlayerID(ssName[playerid]);
	new szTitle[180];
	format(szTitle, sizeof(szTitle), "%s", ssName[playerid]);
	if(userid != INVALID_PLAYER_ID) Dialog_Show(playerid, DIALOG_FRIENDS2, DIALOG_STYLE_LIST, szTitle, "Call\nSMS\n{FF0000}Remove Friend", "Select", "Back");
	else Dialog_Show(playerid, DIALOG_FRIENDS3, DIALOG_STYLE_LIST, szTitle, "{FF0000}Remove Friend", "Select", "Back");
	return 1;
}
Dialog:DIALOG_FRIENDS1(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new id;
	if(sscanf(inputtext, "u", id)) {
		if(!IsPlayerConnected(id)) return SCM(playerid, COLOR_WHITE, "Acel player nu este online!");
	}
	else {
		if(id == playerid) return SCM(playerid, COLOR_GREY, "Nu te poti adauga pe tine in lista!");
		if(!IsPlayerConnected(id)) return SCM(playerid, COLOR_WHITE, "Acel player nu este online!");
		new test, friends;
		new result[180];
		new string[144];
		format(string, sizeof(string), "SELECT * FROM `friends` WHERE `friendID`='%d'", PlayerInfo[id][pSQLID]);
		new Cache: db = mysql_query (SQL, string);
		for(new i, j = cache_num_rows(); i != j; ++i) {
			cache_get_value_name( i, "AddBy", result ); test = strval( result );
			if(test == PlayerInfo[playerid][pSQLID]) return SCM(playerid, COLOR_WHITE, "Acest player este deja in lista ta de prieteni!");
			friends++;
		}
		/*if(friends == 25 && PlayerInfo[playerid][pPremiumAccount] == 0) return SCM(playerid, COLOR_WHITE, "* Poti avea doar 25 prieteni fara cont premium!");
		if(friends == 50) return SCM(playerid, COLOR_WHITE, "* Ai prea multi prieteni!");*/
		cache_delete( db );
		format(string, sizeof(string), "%s (%d) face acum parte din lista ta de prieteni.", GetName(id), id);
		SCM(playerid, COLOR_GOLD, string);
		format(string, sizeof(string), "%s (%d) te-a adaugat in lista sa de prieteni.", GetName(playerid), playerid);
		SCM(id, COLOR_GOLD, string);
		finishAchievement(playerid, 10);	
		format(string, sizeof(string), "INSERT INTO `friends` (`friendID`, `friendName`, `AddBy`) VALUES ('%d', '%s', '%d')", PlayerInfo[id][pSQLID], PlayerInfo[id][pUsername], PlayerInfo[playerid][pSQLID]);
		mysql_tquery(SQL, string, "", "");
	}
	return 1;
}
Dialog:DIALOG_FRIENDS2(playerid, response, listitem, inputtext[]) {
	if(!response) return ShowPlayerFriends(playerid);
	new userid = GetPlayerID(ssName[playerid]);
	if(!IsPlayerConnected(userid)) return true;
	new string[144];
	switch(listitem) {
		case 0: {
			if(PlayerInfo[playerid][pPhone] == 0) return SCM(playerid, -1, "Nu ai a cellphone!");
			foreach(new i: Player) {
				if(PlayerInfo[i][pPhone] == PlayerInfo[userid][pPhone]) {						
					new giveplayerid = i, giveplayer[MAX_PLAYER_NAME];
					if(PlayerInfo[giveplayerid][pPhone] == 0) return SCM(playerid, -1, "Acel player nu are telefon!");
					if(!IsPlayerConnected(giveplayerid)) return true;
					if(giveplayerid == INVALID_PLAYER_ID) return true;
					if(IsBlocked(giveplayerid, playerid)) return SCM(playerid, COLOR_LGREEN, "Acel player te-a blocat! Nu il poti contacta.");
					if(IsBlocked(playerid, giveplayerid)) return SCM(playerid, COLOR_LGREEN, "L-ai blocat pe acel player! Nu il poti contacta.");
					if(PhoneOnline[giveplayerid] > 0) return SCM(playerid, -1, "Are telefonul inchis.");
					if(Mobile[giveplayerid] != 255) return true;
					Mobile[playerid] = giveplayerid;
					
					format(string, sizeof(string), "%s (%s) te suna. Scrie '/pickup' pentru a raspunde.", GetName(playerid), DecimalNumber(playerid));
					SCM(giveplayerid, COLOR_YELLOW, string);
					new tel[180], tel2[180];
					format(tel, 256, "%d", PlayerInfo[playerid][pPhone]);
					format(tel2, 256, "%d", PlayerInfo[giveplayerid][pPhone]);								
					if(strlen(tel) == 4) format(string, sizeof(string), "%s scoate iPhone-ul si scrie un numar.", GetName(playerid));
					else format(string, sizeof(string), "%s scoate telefonul si scrie un numar.", GetName(playerid));		
					ProxDetector(30.0, playerid, string, COLOR_PURPLE);
					GetPlayerName(giveplayerid,giveplayer,sizeof(giveplayer));
					if(PlayerInfo[playerid][pPhone] == 0) SetPlayerAttachedObject(playerid,4,330,6, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000, 1.000000, 1.000000);
					else if(PlayerInfo[playerid][pPhone] == 1) SetPlayerAttachedObject(playerid, 4, 18865, 6, 0.086043, 0.027719, 0.003817, 95.232246, 178.651031, 1.691840, 1.002077, 1.000000, 1.000000); // MobilePhone1 - phone1
					else if(PlayerInfo[playerid][pPhone] == 2) SetPlayerAttachedObject(playerid,4, 18874, 6, 0.086043, 0.027719, 0.003817, 95.232246, 178.651031, 1.691840, 1.002077, 1.000000, 1.000000); // MobilePhone10 - phone10
					if(strlen(tel2) == 4) format(string, sizeof(string), "* Suna iPhone-ul lui %s.", giveplayer);
					else format(string, sizeof(string), "* Suna telefonul lui %s.", giveplayer);
					ProxDetector(20.0, giveplayerid, string, COLOR_PURPLE);
					CellTime[playerid] = 1;
					Update(playerid, pCashx);
					SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
					if(PlayerInfo[playerid][pPhone] == 0) SetPlayerAttachedObject(playerid,4,330,6, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000, 1.000000, 1.000000 );
					else if(PlayerInfo[playerid][pPhone] == 1) SetPlayerAttachedObject( playerid, 4, 18865, 6, 0.086043, 0.027719, 0.003817, 95.232246, 178.651031, 1.691840, 1.002077, 1.000000, 1.000000 ); // MobilePhone1 - phone1
					else if(PlayerInfo[playerid][pPhone] == 2) SetPlayerAttachedObject( playerid,4, 18874, 6, 0.086043, 0.027719, 0.003817, 95.232246, 178.651031, 1.691840, 1.002077, 1.000000, 1.000000 ); // MobilePhone10 - phone10	
				}			
			}
		}
		case 1: {
			if(PlayerInfo[playerid][pPhone] == 0) return SCM(playerid, -1, "Nu ai a cellphone!");
			Dialog_Show(playerid, DIALOG_FRIENDS4, DIALOG_STYLE_INPUT, "SMS:", "Scrie mai jos mesajul pe care vrei sa-l trimiti.", "Ok", "Exit");
		}
		case 2: {
			mysql_format(SQL, string,128,"DELETE FROM `friends` WHERE `friendID`='%d' AND `AddBy`='%d'",PlayerInfo[userid][pSQLID], PlayerInfo[playerid][pSQLID]);
			mysql_tquery(SQL, string, "", "");
			format(string, sizeof(string), "%s nu mai face parte din lista ta de prieteni.", PlayerInfo[userid][pUsername]);
			SCM(playerid, COLOR_GOLD, string);
		}
	}
	return 1;
}
Dialog:DIALOG_FRIENDS3(playerid, response, listitem, inputtext[]) {
	if(!response) return ShowPlayerFriends(playerid);
	new string[144];
	mysql_format(SQL, string,128,"DELETE FROM `friends` WHERE `friendName`='%s' AND `AddBy`='%d'",ssName[playerid], PlayerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, string, "", "");
	format(string, sizeof(string), "%s nu mai face parte din lista ta de prieteni.", ssName[playerid]);
	SCM(playerid, COLOR_GOLD, string);
	return 1;
}
Dialog:DIALOG_FRIENDS4(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new userid = GetPlayerID(ssName[playerid]);
	if(!IsPlayerConnected(userid)) return true;
	foreach(new i: Player){
		if(PlayerInfo[i][pPhone] == PlayerInfo[userid][pPhone] && PlayerInfo[userid][pPhone] != 0) {
			new giveplayerid = i;
			new string[144], sendername[MAX_PLAYER_NAME];
			if(PhoneOnline[giveplayerid] > 0) return SCM(playerid, -1, "Telefonul lui este inchis.");
			GetPlayerName(playerid, sendername, sizeof(sendername));
			if(PlayerInfo[giveplayerid][pAdmin] >= 1) {
				format(string, sizeof(string), "SMS de la %s (%d): %s",GetName(playerid),playerid, inputtext);
				SCM(giveplayerid, COLOR_YELLOW, string);
			}
			else {
				format(string, sizeof(string), "SMS de la %s (%s): %s",GetName(playerid),DecimalNumber(playerid), inputtext);
				SCM(giveplayerid, COLOR_YELLOW, string);
			}
			format(string, sizeof(string), "SMS trimis lui %s (%s): %s",GetName(giveplayerid),DecimalNumber(giveplayerid),inputtext);
			SCM(playerid, 0xC8E0DFFF, string);
			format(string, sizeof(string), "(sms) %s catre %s: %s", sendername, GetName(giveplayerid), inputtext);
			ChatLog(GetName(playerid), playerid, string);	

			format(string, sizeof(string), "(SMS LOG) %s(%d) catre %s(%d): %s", sendername, playerid, GetName(giveplayerid), giveplayerid, inputtext);
			if(PlayerInfo[playerid][pAdmin] < 6) EarsMessage(0xFFC400FF, string);
			PlayerPlaySound(giveplayerid, 1138, 0.0, 0.0, 0.0);
			Reply[giveplayerid] = playerid;
		}
	}
	return 1;
}		
Dialog:DIALOG_FACTIONS(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new i = listitem+1;
	new string[144];
	format(string, sizeof(string), "Factiune: %s\nLider: %s\nMembrii: %s/7\nAplicatii: %s", FactionName(i), GetLeader(i), GetFactionMembers(i), GetFactionStatus(i));
	Dialog_Show(playerid, DIALOG_FACTIONS1, DIALOG_STYLE_MSGBOX, "Info", string, "HQ", "Close");
	SelectedFaction[playerid] = i;
	return 1;
}
Dialog:DIALOG_FACTIONS1(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(CP[playerid] != 0 || targetfind[playerid] != -1) return Dialog_Show(playerid, DIALOG_CHECKPOINT, DIALOG_STYLE_MSGBOX, "Checkpoint", "Ai deja un checkpoint activ.\nDoresti sa-l anulezi? Daca da, apasa pe 'Ok'.", "Ok", "Exit");
	new i = SelectedFaction[playerid];
	SetPlayerCheckpointEx(playerid,DynamicFactions[i][extX], DynamicFactions[i][extY], DynamicFactions[i][extZ],4.0);
	SCM(playerid,COLOR_YELLOW, "Server-ul ti-a pus un punct rosu la locatia aleasa de tine.");
	CP[playerid] = 43;			
	return 1;
}
Dialog:DIALOG_HRADIO(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new i = listitem;
	new query[144];
	HouseInfo[PlayerInfo[playerid][pHouse]][hRadio] = i;
	mysql_format(SQL, query, sizeof(query), "UPDATE `houses` SET `Radio`='%d' WHERE `ID`='%d'", i, PlayerInfo[playerid][pHouse]);
	mysql_tquery(SQL, query, "", "");
	SCM(playerid, COLOR_YELLOW, "Modificari actualizate!");
	if(i == 0) return StopHouseRadio(PlayerInfo[playerid][pHouse]);			
	foreach(new x: Player) {
		if(InHouse[x] == PlayerInfo[playerid][pHouse]) {
			PlayAudioStreamForPlayer(x, RadioLink[HouseInfo[PlayerInfo[playerid][pHouse]][hRadio]]);
			PlayRadio[x] = i;
		}
	}
	return 1;
}
Dialog:DIALOG_BRADIO(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new i = listitem, query[200];
	BizzInfo[PlayerInfo[playerid][pBizz]][bRadio] = i;
	mysql_format(SQL,query, sizeof(query), "UPDATE `bizz` SET `Radio`='%d' WHERE `ID`='%d'", i, PlayerInfo[playerid][pBizz]);
	mysql_tquery(SQL, query, "", "");
	SCM(playerid, COLOR_YELLOW, "Modificari actualizate!");
	if(i == 0) return StopBizRadio(PlayerInfo[playerid][pBizz]);			
	foreach(new x: Player) {
		if(InBussines[x] == PlayerInfo[playerid][pBizz]) {
			PlayAudioStreamForPlayer(x, RadioLink[BizzInfo[PlayerInfo[playerid][pBizz]][bRadio]]);
			PlayRadio[x] = i;
		}
	}
	return 1;
}	
Dialog:DIALOG_PCOLOR(playerid, response, listitem, inputtext[]) {
	if(!response) 
		return true;		

	PlayerInfo[playerid][pColor] = listitem;
	UpdateVar(playerid, "Color", listitem);
	if(listitem == 0) return SCM(playerid, -1, "Ai scos culoarea!");
	format(gString, 75, "(Premium Colors) Culoarea aleasa de tine este {%s}aceasta{FFADAD}.", PlayerColors(listitem));
	SCM(playerid, 0xFFADADFF, gString);
	return 1;
}	
Dialog:DIALOG_VOTE(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	MapVotes[PaintType[playerid]-1][listitem] ++;
	Voted[playerid] = 1;
	new string[144];
	format(string, sizeof(string), "%s a votat pentru mapa %s.", GetName(playerid), MapName2(listitem));
	SendPaintMessage(PaintType[playerid], COLOR_CLIENT, string);
	return 1;
}			
Dialog:DIALOG_CARRADIO(playerid, response, listitem, inputtext[]) {
	if(!response) return 1;
	new i = listitem;
	if(i == 0) return StopAudioStreamForPlayersInCar(GetPlayerVehicleID(playerid)), CarRadio[GetPlayerVehicleID(playerid)] = 0;		
	foreach(new x: Player) {
		if(IsPlayerInAnyVehicle(x) && GetPlayerVehicleID(x) == GetPlayerVehicleID(playerid)) PlayRadio[x] = i;
	}			
	if(i == sizeof(RadioLink)) {
		Dialog_Show(playerid, DIALOG_CARRADIO2, DIALOG_STYLE_INPUT, "Youtube custom URL", "Scrie mai jos link-ul de la videoclipul de pe YouTube:", "Ok", "Exit");
	}
	else {							
		new string[144];
		PlayAudioStreamForPlayersInCar(GetPlayerVehicleID(playerid), RadioLink[i]);
		format(string, sizeof(string), "* %s asculta acum %s.", GetName(playerid), RadioNames[i]);
		NearMessage(playerid, COLOR_PURPLE, string);	
		CarRadio[GetPlayerVehicleID(playerid)] = i;
	}	
	return 1;
}		
Dialog:DIALOG_CARRADIO2(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(!IsPlayerInAnyVehicle(playerid)) return true;
	if(strfind(inputtext, "xnxx", true) != -1) { }
	else return SCM(playerid, COLOR_GREY, "Invalid link!");			
	new k = sizeof(RadioLink);	
	new string[144];	
	format(string, sizeof(string), "%s", inputtext);
	foreach(new x: Player) {
		if(IsPlayerInAnyVehicle(x) && GetPlayerVehicleID(x) == GetPlayerVehicleID(playerid)) {
			PlayRadio[x] = k; 
			PlayAudioStreamForPlayer(x, string);
		}	
	}			
	CarRadio[GetPlayerVehicleID(playerid)] = k;
	format(CarLink[GetPlayerVehicleID(playerid)], 256, string);
	return 1;
}
Dialog:DIALOG_BID(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(Licitatie == 0) return SCM(playerid, -1, "Nu este o licitatie activa!");
	new suma = SelectedMoney[playerid];
	if(PlayerMoney(playerid, TotalBani)) return SCM(playerid, -1, "Nu ai destui bani!");
	if(PlayerMoney(playerid, suma)) return SCM(playerid, -1, "Nu ai destui bani!");
	new string[144];
	if(suma < TotalBani+50000 && TotalBani != 0) {
		format(string, sizeof(string), "Suma invalida! Poti licita minim $%s.", FormatNumber(TotalBani+50000));
		SCM(playerid, COLOR_GREY, string);
		return true;
	}				
	TotalBani = suma;
	TotalMoney[playerid] = suma;
	LastPlayer = playerid;
	if(toglicitatie[playerid] == 0) {
		toglicitatie[playerid] = 1;
		SCM(playerid, COLOR_YELLOW, "Server-ul ti-a activat automat optiunea de a vedea mesajele de la licitatie.");
		UpdateVar(playerid, "TogLicitatie", 1);
	}
	format(string, sizeof(string), "[BID] %s a licitat suma de $%s.", GetName(playerid), FormatNumber(suma));
	MesajLicitatie(0x7A45FFFF, string);	
	return 1;
}	
Dialog:DIALOG_QUITFACTION(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new string2[180];
	new id = playerid, reason;
	
	new szQuery[256];
	format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `Leader`='%d'", PlayerInfo[playerid][pMember]);
	new Cache: result = mysql_query(SQL, szQuery);
	reason = cache_num_rows();
	cache_delete(result);
	FactionMembers[PlayerInfo[playerid][pMember]] --;
	new str[180], query[200];
	if(reason != 0) {
		format(string2, sizeof(string2), "%s a iesit din factiunea %s (rank %d) dupa %d zile (/quitfaction) cu 30 FP.", GetName(playerid),FactionName(PlayerInfo[playerid][pMember]),PlayerInfo[playerid][pRank],PlayerInfo[playerid][pDays]);
		mysql_format(SQL, query, sizeof(query), "INSERT INTO faction_logs (`text`,`player`,`leader`) VALUES ('%s','%d','%d')", string2, PlayerInfo[playerid][pSQLID], PlayerInfo[playerid][pSQLID]);
		mysql_tquery(SQL,query,"","");
		PlayerInfo[playerid][pMember]=0;
		PlayerInfo[playerid][pLeader]=0;
		PlayerInfo[playerid][pRank] = 0;
		PlayerInfo[playerid][pChar] = 0;
		PlayerInfo[playerid][pFACWarns] = 0;
		PlayerInfo[playerid][pFpunish] = 30;
		mysql_format(SQL, str,sizeof(str),"UPDATE users SET `Member`='0',`Leader`='0',`Rank`='0',`CChar`='0',`FWarn`='0',`FPunish`='20',`Team`='3',`Days`='0' WHERE `ID`='%d'",PlayerInfo[playerid][pSQLID]);
		mysql_tquery(SQL, query, "", "");
	}
	else {
		format(string2, sizeof(string2), "%s a iesit din factiunea %s (rank %d) dupa %d zile (/quitfaction) fara fp, motiv: Factiune fara lider.", GetName(playerid),FactionName(PlayerInfo[playerid][pMember]),PlayerInfo[playerid][pRank],PlayerInfo[playerid][pDays]);
		mysql_format(SQL, query, sizeof(query), "INSERT INTO faction_logs (`text`,`player`,`leader`) VALUES ('%s','%d','%d')", string2, PlayerInfo[playerid][pSQLID], PlayerInfo[playerid][pSQLID]);
		mysql_tquery(SQL,query,"","");
		PlayerInfo[playerid][pMember]=0;
		PlayerInfo[playerid][pLeader]=0;
		PlayerInfo[playerid][pRank] = 0;
		PlayerInfo[playerid][pChar] = 0;
		PlayerInfo[playerid][pFACWarns] = 0;
		PlayerInfo[playerid][pFpunish] = 0;				
		mysql_format(SQL, str,sizeof(str),"UPDATE users SET `Member`='0',`Leader`='0',`Rank`='0',`CChar`='0',`FWarn`='0',`FPunish`='0',`Team`='3',`Days`='0' WHERE `ID`='%d'",PlayerInfo[playerid][pSQLID]);
		mysql_tquery(SQL,str, "", "");
		SCM(playerid, COLOR_YELLOW, "Nu ai primit FPunish deoarece nu este lider!");
	}
	
	gTeam[playerid] = 3;
	PlayerInfo[playerid][pTeam] = 3;
	OnDuty[playerid] = 0;
	tazer[playerid] = 0;
	SetPlayerArmourEx(id, 0);
	SetPlayerSkinEx(id,PlayerInfo[playerid][pModel]);
	SetPlayerToTeamColor(id);
	PlayerInfo[playerid][pCommands][0] = 0;
	new rquery[256];

	mysql_format(SQL, rquery, sizeof(rquery), "UPDATE `users` SET `Commands`='%d' WHERE `ID`='%d'", PlayerInfo[playerid][pCommands][0], PlayerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, rquery, "", "");
	SpawnPlayer(id);
	return 1;
}			
Dialog:DIALOG_MODE(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new string[144];
	PaintType[playerid] = listitem+1;
	PKills[playerid] = 0, PDeaths[playerid] = 0;
	format(string, sizeof(string), "%s a intrat in arena de paintball.", GetName(playerid));
	SendPaintMessage(listitem+1, COLOR_YELLOW, string);
	PlayerTextDrawHide(playerid, PaintTD);
	ResetPlayerWeapons(playerid);
	PaintSpawn(playerid);

	foreach(new i: Player) {
		if(PaintType[i] != 0) PKills[i] = 0, PDeaths[i] = 0;
	}
	if(PlayersOnPaint(listitem) == 2) {	
		PaintTime[listitem] = -1;
		PaintText[listitem] = 60;
		SendPaintMessage(listitem+1, COLOR_YELLOW, "Runda va incepe peste un minut. Pentru a vota o mapa, folositi comanda /votemap.");
		PaintVote[listitem] = 1;	
		if(PaintTimer[listitem] != 0) KillTimer(PaintTimer[listitem]), PaintTimer[listitem] = 0;
		PaintTimer[listitem] = SetTimerEx("CalculateVote", 60000, 0, "i", listitem);
	}
	else if(PlayersOnPaint(listitem) < 2) SCM(playerid, COLOR_YELLOW, "Pentru a incepe runda, mai este nevoie de inca un jucator.");
	return 1;
}
Dialog:DIALOG_CHECKPOINT(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(CP[playerid] == 50) return SCM(playerid,COLOR_WHITE, "Acel checkpoint este pentru rob.");
	if(InExamen[playerid] == 1) FailExamen(playerid);
	DisablePlayerRaceCheckpointEx(playerid);
	targetfind[playerid] = -1;
	CP[playerid] = 0;
	UsedFind[playerid] = 0;		

	DisablePlayerCheckpointEx(playerid);
	SCM(playerid,COLOR_WHITE,"Punctul rosu a fost sters cu succes.");
	PlayerTextDrawHide(playerid, FindTD);
	PlayerTextDrawHide(playerid, FindSkinTD);
	PlayerTextDrawHide(playerid, JobTD);
	PlayerTextDrawHide(playerid, InfosTD);
	DisablePlayerCheckpointEx(playerid);	
	if(JobWorking[playerid] == 1) {
		SCM(playerid, COLOR_GREY, "Job esuat!");
		SetPlayerSpecialAction(playerid, 0);
		if(playerObject[playerid] != 0) DestroyPlayerObject(playerid, playerObject[playerid]), playerObject[playerid] = 0;
		if(playerObject2[playerid] != 0) DestroyObject(playerObject2[playerid]), playerObject2[playerid] = 0;
		JobWorking[playerid] = 0;
		PlayerTextDrawHide(playerid, JobTD);
		if(JobVehicle[playerid] != 0) { DestroyVehicle(JobVehicle[playerid]); JobVehicle[playerid] = 0; }
		if(Trailer[playerid] != 0) { DestroyVehicle(Trailer[playerid]); Trailer[playerid] = 0; }
		PlayerTextDrawHide(playerid, InfosTD);
		DisablePlayerCheckpointEx(playerid);
		DisablePlayerRaceCheckpointEx(playerid);
		CP[playerid] = 0;
		JobVehicle[playerid] = 0;
		JobDeelay[playerid][PlayerInfo[playerid][pJob]] = 180;
		SetPlayerSpecialAction(playerid, 0);
	}	
	JobLeftTime[playerid] = 0;
	return 1;
}
Dialog:DIALOG_TOG(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	switch(listitem) {
		case 0: {
			if(NewbieChat[playerid] == 0) {
				if(PlayerInfo[playerid][pHelper] >= 1) return SCM(playerid, COLOR_GREY, "Nu poti dezactiva deoarece esti helper.");
				NewbieChat[playerid] = 1;
			}
			else NewbieChat[playerid] = 0;
			UpdateVar(playerid, "NewbieChat", NewbieChat[playerid]);
		}
		case 1: {
			if(PlayerInfo[playerid][pLeader] == 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu esti lider!");
			if(toglc[playerid] == 0) toglc[playerid] = 1;
			else toglc[playerid] = 0;
			UpdateVar(playerid, "TogLC", toglc[playerid]);
		}
		case 2: {
			if(PlayerInfo[playerid][pMember] == 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu esti membru al unei factiuni!");
			if(gFam[playerid] == 0) gFam[playerid] = 1;
			else gFam[playerid] = 0;
			UpdateVar(playerid, "TogFC", gFam[playerid]);
		}
		case 3: {
			if(WTToggle[playerid] == 0) WTToggle[playerid] = 1;
			else WTToggle[playerid] = 0;
			UpdateVar(playerid, "TogWT", WTToggle[playerid]);
		}
		case 4: {
			if(HidePM[playerid] == 1) HidePM[playerid] = 0;
			else HidePM[playerid] = 1;
			UpdateVar(playerid, "HidePM", HidePM[playerid]);
		}
		case 5: {
			if(gOoc[playerid] == 0) gOoc[playerid] = 1;
			else gOoc[playerid] = 0;
		}
		case 6: {
			if(gNews[playerid] == 0) gNews[playerid] = 1;
			else gNews[playerid] = 0;
			UpdateVar(playerid, "TogNews", gNews[playerid]);
		}
		case 7: {
			if(toglicitatie[playerid] == 0) toglicitatie[playerid] = 1, SCM(playerid, COLOR_YELLOW, "Pentru a vedea informatii despre licitatie, foloseste /info.");
			else toglicitatie[playerid] = 0;	
			UpdateVar(playerid, "TogLicitatie", toglicitatie[playerid]);		
		}
		case 8: {
			if(togclan[playerid] == 0) togclan[playerid] = 1;
			else togclan[playerid] = 0;
			UpdateVar(playerid, "TogClan", togclan[playerid]);	
		}		
		case 9: {
			if(PlayerInfo[playerid][pAdmin] < 5) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu poti folosi acest item!");
			if(GetPVarInt(playerid, "Undercover") == 0) {
				SetPVarInt(playerid, "Undercover", 1);
				SCM(playerid, COLOR_WHITE, "Acum esti sub acoperire.");
				foreach(new i: Player) ShowPlayerNameTagForPlayer(playerid, i, false);
			}
			else {
				SetPVarInt(playerid, "Undercover", 0);
				SCM(playerid, COLOR_WHITE, "Acum nu mai esti sub acoperire.");
				foreach(new i: Player) ShowPlayerNameTagForPlayer(playerid, i, true);
			}				
		}
		case 10: {
			if(togevent[playerid] == 0) togevent[playerid] = 1;
			else togevent[playerid] = 0;
			UpdateVar(playerid, "TogEvent", togevent[playerid]);
		}	
		case 11: {
			if(togding[playerid] == 0) togding[playerid] = 1;
			else togding[playerid] = 0;		
			UpdateVar(playerid, "TogDing", togding[playerid]);		
		}
		case 12: {
			if(togsurf[playerid] == 0) togsurf[playerid] = 1;
			else togsurf[playerid] = 0;		
			UpdateVar(playerid, "TogSurf", togsurf[playerid]);		
		}	
		case 13: {
			if(togfind[playerid] == 0) togfind[playerid] = 1;
			else togfind[playerid] = 0;		
			UpdateVar(playerid, "TogFind", togfind[playerid]);		
		}	
		case 14: {
			if(tograport[playerid] == 0) tograport[playerid] = 1;
			else tograport[playerid] = 0;		
			UpdateVar(playerid, "TogRaport", tograport[playerid]);					
		}
		case 15: {
			if(togjob[playerid] == 0) togjob[playerid] = 1;
			else togjob[playerid] = 0;		
			UpdateVar(playerid, "TogJob", togjob[playerid]);					
		}				
		case 16: {
			if(PlayerInfo[playerid][pBackpack] == 2) {
				RemovePlayerAttachedObject(playerid, 8);
				PlayerInfo[playerid][pBackpack] = 1;
			}
			else if(PlayerInfo[playerid][pBackpack] == 1) {
				SetPlayerAttachedObject(playerid, 8, 3026, 1, -0.172813, -0.112569, -0.022387, 0.000000, 0.000000, 3.390071, 1.000000, 1.000000, 1.130721);
				PlayerInfo[playerid][pBackpack] = 2;
			}
			Update(playerid, pBackpackx);
		}
		case 17: {
			if(togvip[playerid] == 0) togvip[playerid] = 1;
			else togvip[playerid] = 0;
			UpdateVar(playerid, "TogVip", togvip[playerid]);
		}
		case 18: {
			if(togpremium[playerid] == 0) togpremium[playerid] = 1;
			else togpremium[playerid] = 0;
			UpdateVar(playerid, "TogPremium", togpremium[playerid]);
		}
		case 19: {
			Dialog_Show(playerid, DIALOG_TOGRAINBOW, DIALOG_STYLE_INPUT, "Rainbow speed tog", "You type rainbow speed.", "Done", "Cancel");
			UpdateVar(playerid, "TogRainBow", tograinbow[playerid]);
		}
	}
	SCM(playerid, 0xFFADADFF, "Modificari actualizate!");
	return 1;
}
Dialog:DIALOG_TOGRAINBOW(playerid, response, listitem, inputtext[]) {
	new speed = strval(inputtext);
	if(speed > 3) return SCM(playerid, COLOR_GREY, "Invalid.");
	new string[144];
	format(string, sizeof(string), "Rainbow speed is now %d.", strval(inputtext));
	SCM(playerid, COLOR_YELLOW, string);
	tograinbow[playerid] = speed;
	UpdateVar(playerid, "TogRainBow", speed);
	return 1;
}		
Dialog:DIALOG_FACTION(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	SelectedRank[playerid] = listitem;
	Dialog_Show(playerid, DIALOG_FACTION1, DIALOG_STYLE_INPUT, "Raport system:", "Introdu numarul de comenzi pe care vrei sa-l faca membrii de acest rank!", "Select", "Back");
	return 1;
}
Dialog:DIALOG_FACTION1(playerid, response, listitem, inputtext[]) {
	new ffID = GetPVarInt(playerid, "Faction");
	new string[144], query[250];
	if(!response) {
		format(string, sizeof(string), "Rank 1\t%d\nRank 2\t%d\nRank 3\t%d\nRank 4\t%d\nRank 5\t%d\nRank 6\t%d", FactionInfo[ffID][fRank1], FactionInfo[ffID][fRank2],
		FactionInfo[ffID][fRank3], FactionInfo[ffID][fRank4], FactionInfo[ffID][fRank5], FactionInfo[ffID][fRank6]);
		Dialog_Show(playerid, DIALOG_FACTION, DIALOG_STYLE_TABLIST, "Raport system:", string, "Select", "Iesire");
		return true;
	}
	format(string, sizeof(string), "Numarul de comenzi pe care trebuie sa-l depuna membrii de rank %d din factiunea %s, este acum de: %d", SelectedRank[playerid]+1, FactionName(ffID), strval(inputtext));
	SCM(playerid, -1, string);
	switch(SelectedRank[playerid]) {
		case 0: FactionInfo[ffID][fRank1] = strval(inputtext);
		case 1: FactionInfo[ffID][fRank2] = strval(inputtext);
		case 2: FactionInfo[ffID][fRank3] = strval(inputtext);
		case 3: FactionInfo[ffID][fRank4] = strval(inputtext);
		case 4: FactionInfo[ffID][fRank5] = strval(inputtext);
		case 5: FactionInfo[ffID][fRank6] = strval(inputtext);
	}
	mysql_format(SQL, query, sizeof(query), "UPDATE `factions` SET `Rank1`='%d', `Rank2`='%d', `Rank3`='%d', `Rank4`='%d', `Rank5`='%d', `Rank6`='%d' WHERE `ID`='%d'",
	FactionInfo[ffID][fRank1], FactionInfo[ffID][fRank2], FactionInfo[ffID][fRank3], FactionInfo[ffID][fRank4], FactionInfo[ffID][fRank5], FactionInfo[ffID][fRank6], ffID);
	mysql_tquery(SQL, query, "", "");
	return 1;
}
Dialog:DIALOG_RESETKM(playerid, response, listitem, inputtext[]) {
	if(!response) return ShowShop(playerid);
	if(!IsPlayerInAnyVehicle(playerid)) return SCM(playerid, COLOR_GREY, "Nu esti intr-un vehicul!");
	if(PlayerInfo[playerid][pPremiumPoints] < 35) return SCM(playerid, -1, "Nu ai suficiente Premium Points!");
	PlayerInfo[playerid][pPremiumPoints] -= 35;
	Update(playerid, pPremiumPointsx);	
	CarInfo[pCar[playerid]][KM] = 0;
	new query[144];
	mysql_format(SQL, query, sizeof(query), "SELECT `cars` SET `Km`='0' WHERE `ID`='%d'", CarInfo[pCar[playerid]][cID]);
	mysql_tquery(SQL, query, "", "");
	SCM(playerid, COLOR_GOLD, "{2584CA}(SHOP){FFFFFF} Felicitari! Ti-ai resetat kilometrii vehiculului personal.");
	return 1;
}
Dialog:DIALOG_MEMBERS(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(PlayerInfo[playerid][pRank] < 6) return SCM(playerid, COLOR_LGREEN, "Eroare: Ai nevoie de rank 6+ pentru a putea merge mai departe!");
	new szQuery[256];
	new string[144];
	format(ssName[playerid], 256, Selected[playerid][listitem]);	
	format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `name`='%s'", ssName[playerid]);
	new Cache: result = mysql_query(SQL, szQuery);
	cache_get_value_name(0, "Rank", string);
	new rank = strval(string);
	if(rank == 7)  return SCM(playerid, COLOR_LGREEN, "Eroare: Nu se poate modifica ceva unui lider!");
	cache_delete(result);						
	format(string, sizeof(string), "Member: %s", ssName[playerid]);
	Dialog_Show(playerid, DIALOG_MEMBERS1, DIALOG_STYLE_LIST, string, "Raport\nRank\nFaction warn\nUn faction warn\nUninvite", "Ok", "Back");
	return 1;
}
Dialog:DIALOG_RAPORTS(playerid, response, listitem, inputtext[]) {
	if(!response) return ShowFactionMembers(playerid);
	return 1;
}
Dialog:DIALOG_MEMBERS1(playerid, response, listitem, inputtext[]) {
	new szQuery[256], str[256], MemberString[400], szResult[100], name[MAX_PLAYER_NAME];
	if(!response) return ShowFactionMembers(playerid);
	new userID = GetPlayerID(ssName[playerid]);
	if(PlayerInfo[playerid][pRank] < 6) return SCM(playerid, COLOR_LGREEN, "Eroare: Ai nevoie de rank 6+ pentru a putea merge mai departe!");
	new string[144];
	switch(listitem) {
		case 0: {
			new rank[64], fw[64];
			format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `name`='%s'", ssName[playerid]);
			new Cache: result = mysql_query(SQL, szQuery);
			if(!IsACop(playerid)) {
				new YCMDs[3];
				for(new i, j = cache_num_rows(); i != j; ++i)
				{
					cache_get_value_name(i, "name", szResult); format(name, 256, szResult);
					cache_get_value_name(i, "Commands", szResult); YCMDs[0] = strval(szResult);
					cache_get_value_name(i, "InvalidCommands", szResult); YCMDs[1] = strval(szResult);
					cache_get_value_name(i, "Fires", szResult); YCMDs[2] = strval(szResult);
					cache_get_value_name(i, "Rank", szResult); format(rank, 256, szResult);
					cache_get_value_name(i, "FWarn", szResult);  format(fw, 256, szResult);

					if(PlayerInfo[playerid][pMember] == 13) format(string, sizeof(string), "Name: %s\nRank: %s\nFaction warns: %s\nCommands: %d/%d\nIncendii: %d\nInvalid commands: %d", name, rank, fw, YCMDs[0], GetNeedPointsR2(strval(rank), PlayerInfo[playerid][pMember]), YCMDs[2], YCMDs[1]);
					else format(string, sizeof(string), "Name: %s\nRank: %s\nFaction warns: %s\nCommands: %d/%d\nInvalid commands: %d", name, rank, fw, YCMDs[0], GetNeedPointsR2(strval(rank), PlayerInfo[playerid][pMember]), YCMDs[1]);
					
					strcat(MemberString, string);
				}
				cache_delete(result);
			}
			else {
				new YCMDs[6];
				for(new i, j = cache_num_rows(); i != j; ++i)
				{
					cache_get_value_name(i, "name", szResult); format(name, 256, szResult);
					cache_get_value_name(i, "Commands", szResult); YCMDs[0] = strval(szResult);
					cache_get_value_name(i, "Commands2", szResult); YCMDs[1] = strval(szResult);
					cache_get_value_name(i, "Commands3", szResult); YCMDs[2] = strval(szResult);
					cache_get_value_name(i, "Commands4", szResult); YCMDs[3] = strval(szResult);
					cache_get_value_name(i, "Commands5", szResult); YCMDs[4] = strval(szResult);
					cache_get_value_name(i, "InvalidCommands", szResult); YCMDs[5] = strval(szResult);
					cache_get_value_name(i, "Rank", szResult); format(rank, 256, szResult);
					cache_get_value_name(i, "FWarn", szResult);  format(fw, 256, szResult);

					format(string, sizeof(string), "Name: %s\nRank: %s\nFaction warns: %s\nRunners: %d\nArestari: %d\nAmenzi: %d\nPermise confiscate: %d\nDroguri confiscate: %d\nInvalid commands: %d", name, rank, fw, YCMDs[0], YCMDs[1], YCMDs[2], YCMDs[3], YCMDs[4], YCMDs[5]);
					strcat(MemberString, string);
				}
				cache_delete(result);	
			}
			Dialog_Show(playerid, DIALOG_RAPORTS, DIALOG_STYLE_MSGBOX, "Raports", MemberString, "Ok", "Back");					
		}
		case 1: {
			format(string, sizeof(string), "Scrie mai jos rankul pe care vrei sa il dai membrului %s.", ssName[playerid]);
			Dialog_Show(playerid, DIALOG_MEMBERS2, DIALOG_STYLE_INPUT, "Give rank", string, "Ok", "Close");
		}
		case 2: {					
			format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `name`='%s'", ssName[playerid]);
			new Cache: result = mysql_query(SQL, szQuery);
			cache_get_value_name(0, "FWarn", string);
			new fw = strval(string);
			cache_get_value_name(0, "Days", string);
			new days = strval(string);
			cache_get_value_name(0, "Rank", string);
			new rank = strval(string);
			if(PlayerInfo[playerid][pRank] == 6 && rank >= 6) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu poti da FW unui membru cu rank 6+!");
			cache_get_value_name(0, "id", string);
			new id = strval(string);					
			cache_delete(result);
			fw++;
			
			format(string, sizeof(string), "%s i-a acordat lui %s un Faction Warn.", GetName(playerid), ssName[playerid]);
			SendFactionMessage(PlayerInfo[playerid][pMember], COLOR_CLIENT, string);
			
			mysql_format(SQL,str,128,"UPDATE `users` SET `FWarn`='%d' WHERE `name`='%s' LIMIT 1",fw,ssName[playerid]);
			mysql_tquery(SQL,str, "", "");
			if(userID != INVALID_PLAYER_ID) {
				PlayerInfo[userID][pFACWarns] = fw;						
			}
			if(fw >= 3)
			{
				FactionMembers[PlayerInfo[playerid][pMember]] --;
				format(string, sizeof(string), "Ai fost dat afara din factiunea %s (rank %d) dupa %d zile de %s, motiv: 3/3 Faction Warns", FactionName(PlayerInfo[playerid][pMember]), rank, days, GetName(playerid));
				InsertEmail(ssName[playerid], GetName(playerid), string, 0);
				if(userID != INVALID_PLAYER_ID) {
					PlayerInfo[userID][pFACWarns] = 0;	
					PlayerInfo[userID][pMember] = 0;
					PlayerInfo[userID][pRank] = 0;
					PlayerInfo[userID][pFpunish] = 20;
					PlayerInfo[userID][pDays] = 0;
					PlayerInfo[userID][pChar] = 47;
					PlayerTextDrawHide(userID, WarTime);
					SetPlayerSkinEx(userID, 47);
					SpawnPlayer(userID);
					format(string, sizeof(string), "Ai fost demis din factiunea %s de AdmBot, motiv: 3/3 FW.", FactionName(PlayerInfo[playerid][pMember]));
					Dialog_Show(userID, 0, DIALOG_STYLE_MSGBOX, "Uninvite", string, "Close", "");							
				}					
				format(string, sizeof(string), "%s a fost dat afara de AdmBot, motiv: 3/3 FW.", ssName[playerid]);
				SendFactionMessage(PlayerInfo[playerid][pMember], COLOR_CLIENT, string);
			
				new string2[180], query[250];
				format(string2, sizeof(string2), "%s a fost scos de Lider %s din %s (rank %d) dupa %d zile.", ssName[playerid],GetName(playerid),FactionName(PlayerInfo[playerid][pMember]),rank, days);
				mysql_format(SQL, query, sizeof(query), "INSERT INTO faction_logs (`text`,`player`,`leader`) VALUES ('%s','%d','%d')", string2, id, PlayerInfo[playerid][pSQLID]);
				mysql_tquery(SQL,query,"","");							
				mysql_format(SQL,str,sizeof(str),"UPDATE `users` SET `Member`='0',`Rank`='0',`CChar`='250',`FPunish`='20',`FWarn`='0',`Leader`='0',`Days`='0' WHERE `name`='%s' LIMIT 1",ssName[playerid]);
				mysql_tquery(SQL,str, "", "");
			}
		}
		case 3: {
			format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `name`='%s'", ssName[playerid]);
			new Cache: result = mysql_query(SQL, szQuery);
			cache_get_value_name(0, "FWarn", string);
			new fw = strval(string);
			cache_delete(result);
			if(fw == 0) return SCM(playerid, -1, "Acel membru nu are FW.");
			format(string, sizeof(string), "%s i-a scos lui %s un Faction Warn.", GetName(playerid), ssName[playerid]);
			SendFactionMessage(PlayerInfo[playerid][pMember], COLOR_CLIENT, string);			
			mysql_format(SQL,str,128,"UPDATE `users` SET `FWarn`='%d' WHERE `name`='%s' LIMIT 1",fw-1,ssName[playerid]);
			mysql_tquery(SQL,str, "", "");
		}
		case 4: {
			format(string, sizeof(string), "Uninvite %s", ssName[playerid]);
			Dialog_Show(playerid, DIALOG_REASON, DIALOG_STYLE_INPUT, string, "Scrie mai jos motivul pentru pe care vrei sa-l demiti pe acest membru.", "Ok", "Close");				
		}
	}
	return 1;
}
Dialog:DIALOG_REASON(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new string[144];
	SetPVarString(playerid, "Reason", inputtext);
	format(string, sizeof(string), "Uninvite %s", ssName[playerid]);
	Dialog_Show(playerid, DIALOG_MEMBERS3, DIALOG_STYLE_INPUT, string, "Scrie mai jos numarul de FP-uri pe care vrei sa-i dai acestui membru.\nDaca membrul are peste 7 zile, aceasta poate primi fara FP dar cu exceptii", "Ok", "Close");
	return 1;
}		
Dialog:DIALOG_MEMBERS2(playerid, response, listitem, inputtext[]) {
	new str[180];
	if(!response) return true;
	new userID = GetPlayerID(ssName[playerid]);
	new rank = strval(inputtext);
	if(rank < 1 || rank > 6) return SCM(playerid, -1, "Rank invalid.");
	new string[144];
	format(string, sizeof(string), "%s i-a dat lui %s rank %d.", GetName(playerid), ssName[playerid], rank);
	SendFactionMessage(PlayerInfo[playerid][pMember], COLOR_CLIENT, string);	
	if(userID != INVALID_PLAYER_ID) PlayerInfo[userID][pRank] = rank;
	mysql_format(SQL, str,128,"UPDATE `users` SET `Rank`='%d' WHERE `name`='%s' LIMIT 1",rank,ssName[playerid]);
	mysql_tquery(SQL, str, "", "");
	format(string, sizeof(string), "Got rank %d.", rank);
	FactionLog(PlayerInfo[playerid][pMember],ssName[playerid],string);
	return 1;
}
Dialog:DIALOG_MEMBERS3(playerid, response, listitem, inputtext[]) {
	new str[500], string[150], query[250];
	if(!response) return true;
	new fp = strval(inputtext);
	new userID = GetPlayerID(ssName[playerid]), szQuery[256], reason[64];
	GetPVarString(playerid, "Reason", reason, 64);
	if(userID != INVALID_PLAYER_ID) SCM(playerid, -1, "Acel player este conectat.");
	format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `name`='%s'", ssName[playerid]);
	new Cache: result = mysql_query(SQL, szQuery);
	cache_get_value_name(0, "id", string); new id = strval(string);
	cache_get_value_name(0, "Rank", string); new rank = strval(string);
	cache_get_value_name(0, "Days", string); new days = strval(string);	
	cache_delete(result);
	if(PlayerInfo[playerid][pRank] == 6 && rank >= 6) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu poti da afara un membru cu rank 6+!");
	FactionMembers[PlayerInfo[playerid][pMember]] --;

	format(string, sizeof(string), "Ai fost dat afara din factiunea %s (rank %d) dupa %d zile de %s, motiv: %s.",  FactionName(PlayerInfo[playerid][pMember]), rank, days, GetName(playerid), reason);
	InsertEmail(ssName[playerid], GetName(playerid), string, 0);
	new string2[180];
	format(string2, sizeof(string2), "%s a fost scos de %s din %s (rank %d) dupa %d zile, motiv: %s.", ssName[playerid],GetName(playerid),FactionName(PlayerInfo[playerid][pMember]),rank, days,reason);
	mysql_format(SQL, query, sizeof(query), "INSERT INTO faction_logs (`text`,`player`,`leader`) VALUES ('%s','%d','%d')", string2, id, PlayerInfo[playerid][pSQLID]);
	mysql_tquery(SQL,query,"","");	
	FactionLog(PlayerInfo[playerid][pMember],ssName[playerid],"Left the faction.");
	if(userID != INVALID_PLAYER_ID) {
		PlayerInfo[userID][pFACWarns] = 0;	
		PlayerInfo[userID][pMember] = 0;
		PlayerInfo[userID][pRank] = 0;
		PlayerInfo[userID][pFpunish] = 0;
		PlayerInfo[userID][pDays] = 0;
		PlayerInfo[userID][pChar] = 47;
		PlayerTextDrawHide(userID, WarTime);
		SetPlayerSkinEx(userID, 47);
		SpawnPlayer(userID);
	}
		
	if(fp == 0) {
		format(string, sizeof(string), "%s l-a dat afara pe %s fara faction punish, motiv: %s.", GetName(playerid), ssName[playerid], reason);
		SendFactionMessage(PlayerInfo[playerid][pMember], COLOR_CLIENT, string);	
		if(userID != INVALID_PLAYER_ID) {
			format(string, sizeof(string), "Ai fost demis din factiunea %s de %s fara FP, motiv: %s.", FactionName(PlayerInfo[playerid][pMember]), GetName(playerid), reason);
			Dialog_Show(userID, 0, DIALOG_STYLE_MSGBOX, "Uninvite", string, "Close", "");
		}								
	}
	else {
		format(string, sizeof(string), "%s l-a dat afara pe %s cu faction punish, motiv: %s.", GetName(playerid), ssName[playerid], reason);
		SendFactionMessage(PlayerInfo[playerid][pMember], COLOR_CLIENT, string);	
		if(userID != INVALID_PLAYER_ID) {
			format(string, sizeof(string), "Ai fost demis din factiunea %s de %s cu FP, motiv: %s.", FactionName(PlayerInfo[playerid][pMember]), GetName(playerid), reason);
			Dialog_Show(userID, 0, DIALOG_STYLE_MSGBOX, "Uninvite", string, "Close", "");
		}					
	}
	mysql_format(SQL,str,256,"UPDATE `users` SET `Member`='0',`Rank`='0',`CChar`='250',`FPunish`='%d',`FWarn`='0',`SpawnChange`='0',`Commands`='0',`Leader`='0',`Days`='0' WHERE `name`='%s' LIMIT 1",fp, ssName[playerid]);
	mysql_tquery(SQL,str, "", "");		
	return 1;
}
Dialog:DIALOG_SELLCAR(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new vsid = PlayerSellCar[playerid];
	if(CarInfo[vsid][Spawned] == 0) return true;
				
	new value;
	if(CarInfo[vsid][cSpecial] != 1) value = CarInfo[vsid][cValue] / 2;
	else value = CarInfo[vsid][cValue];
	new string[144];
	GivePlayerCash(playerid, 1, value);
	format(string, sizeof(string), "Ai primit $%s pentru ca ti-ai vandut vehiculul!",FormatNumber(value), "%%");
	SCM(playerid,COLOR_MONEY,string);
				
	format(string, sizeof(string), "%s si-a vandut vehiculul %s[%d] (la stat) si a primit $%s.", GetName(playerid), aVehicleNames[CarInfo[vsid][cModel]-400], vsid, FormatNumber(value));
	SendAdminMessage(COLOR_YELLOW, string, 1); 
	InsertLog(playerid, string, LOG_CAR);	
	DestroyPlayerCar(vsid);
	Iter_Remove(MyVehicle[playerid], vsid);
	Iter_Remove(PersonalVehicles, vsid);
	mysql_format(SQL, gQuery, sizeof(gQuery), "DELETE FROM `cars` WHERE `ID`='%d' LIMIT 1", CarInfo[vsid][cID]);
	mysql_tquery(SQL, gQuery, "", "");
	foreach(new i: Player) {
		if(IsPlayerConnected(i) && IsPlayerLogged[i] == 1) {
			if(GetPVarInt(i, "OfferBy") == playerid) {
				SCM(playerid, -1, "Afacerea a fost anulata!");
				SCM(i, -1, "Afacerea a fost anulata!");
				SetPVarInt(i, "OfferBy", -1);		
			}
			if(TradePlayer[i] == playerid) {
				SCM(playerid, -1, "Afacerea a fost anulata!");
				SCM(i, -1, "Afacerea a fost anulata!");				
				TradePlayer[i] = -1;
			}
		}
	}			
	return 1;
}
Dialog:DIALOG_GARAGE(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new stringg[700], idd = PlayerInfo[playerid][pCarKey][listitem];
	listitemm[playerid] = idd;
	strcat(stringg,"Option\tMore detalis\n");
	strcat(stringg,"Vehicle Info\tSee vehicle detalis.\n");
	strcat(stringg,"Find Vehicle\tLocate your vehicle.\n"); 
	strcat(stringg,"Tow vehicle\tRespawn your vehicle.\n");
	if(CarInfo[idd][Spawned] != 0) strcat(stringg,"Despawn\tDestroy your vehicle.\n");
	else strcat(stringg,"Spawn\tSpawn your vehicle.\n");
	strcat(stringg,"Debug\tSpawn your vehicle in another locations.\n"); 
	strcat(stringg,"Sell vehicle\tSell vehicle to Dealership (50).\n");
	strcat(stringg,"Buy insurance\tBuy insurance points.\n");
	strcat(stringg, strlen(CarInfo[idd][cText]) < 3 ? "Upgrade To VIP Vehicle\t{51dd32}300 premium points\n" : "Upgrade To VIP Vehicle\t{cf2424}300 premium points\n");
	strcat(stringg, CarInfo[idd][cVip] != 1 ? "Upgrade To VIP PLUS\t{51dd32}500 premium points\n" : "Teleport Vehicle\t{32e4ea}get vehicle to you\n");
	strcat(stringg, !CarInfo[idd][cRainBow] ? "Upgrade To Rainbow Vehicle\t{51dd32}300 premium points\n" : "Upgrade To Rainbow Vehicle\t{cf2424}300 premium points\n");
	strcat(stringg,"{FF0000}Remove tuning\t{FFFFFF}Remove tuning from your vehicle.");
	if(CarInfo[idd][Confiscated] == 1) strcat(stringg, "\n{FFFFFF}Pay fine to TTC");
	Dialog_Show(playerid, DIALOG_GARAGE1, DIALOG_STYLE_TABLIST_HEADERS, "Options", stringg, "Select", "Cancel");
	return 1;
}
Dialog:DIALOG_STICKET(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	new idd = PlayerInfo[playerid][pCarKey][listitem];
	listitemm[playerid] = idd;
	format(gString, 100, "Rainbow ticket\t{FFFF00}%d\nVip ticket\t{211CDE}%d", PlayerInfo[playerid][pTickete][0], PlayerInfo[playerid][pTickete][1]);
	Dialog_Show(playerid, DIALOG_SELECTC, DIALOG_STYLE_LIST, "Select ticket", gString, "Select", "Close");
	return 1;
}
Dialog:DIALOG_SELECTC(playerid, response, listitem, inputtext[]) {
	if(!response) 
		return ShowPlayerVehicle(playerid, true);

	if(PlayerInfo[playerid][pTickete][listitem] < 1) 
		return SCM(playerid, COLOR_LGREEN, "Nu ai acest tip de ticket!");
	new string[144];
	new idd = listitemm[playerid];
	switch(listitem) {
		case 0: {
			if(CarInfo[idd][cRainBow]) return SCM(playerid, COLOR_LGREEN, "Acest vehicul este deja upgradat Rainbow.");
			SCM(playerid, COLOR_LGREEN, "Ticket: {FFFFFF}Ti-ai upgradat vehiculu Rainbow, pentru un ticket Rainbow.");
			mysql_format(SQL, string, sizeof(string), "UPDATE `cars` SET `Rainbow`='1' WHERE `ID`='%d'", CarInfo[idd][cID]);
			mysql_tquery(SQL, string, "", "");
			CarInfo[idd][cRainBow] = true;					
		}
		case 1: {
			if(strlen(CarInfo[idd][cText]) > 3) return SCM(playerid, COLOR_LGREEN, "Eroare: Acest vehicul este deja upgradat la VIP!");
			SCM(playerid, COLOR_LGREEN, "Ticket: {FFFFFF}Ti-ai upgradat vehiculu VIP, pentru un ticket Vip.");
			mysql_format(SQL, string, sizeof(string), "UPDATE `cars` SET `Text`='%s' WHERE `ID`='%d'", GetName(playerid), CarInfo[idd][cID]);
			mysql_tquery(SQL, string, "", "");
			format(CarInfo[idd][cText], 32, GetName(playerid));
			attach_vip_text(idd);
		}
	}
	PlayerInfo[playerid][pTickete][listitem] --;	
	save_tickete(playerid);	
	return 1;
}
Dialog:DIALOG_EDITCARS(playerid, response, listitem, inputtext[]) {
	if(PlayerInfo[playerid][pAdmin] < 7) return true;
	if(!response) return true;
	EditVehicleID[playerid] = PlayerInfo[playerid][pCarKey][listitem];
	Dialog_Show(playerid, DIALOG_EDITCARS1, DIALOG_STYLE_TABLIST_HEADERS, "Options", "Option\nVehicle Info\nEdit Others", "Ok", "Back");
	return 1;
}
Dialog:DIALOG_EDITCARS1(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new vsid = EditVehicleID[playerid];
	new string[144];	
	switch(listitem) {
		case 0: {
			new 
				locktext[40], Legendary[20], vip[20], rainbow[20], stage[35], Other[95],
				Culoare1 = CarInfo[vsid][cColorOne], Culoare2 = CarInfo[vsid][cColorTwo]
			;
			if(CarInfo[vsid][cLegendary] == 1) { Legendary = " {F28644}[P]"; }
			else if(CarInfo[vsid][cLegendary] == 0) { Legendary = ""; }
			if(strlen(CarInfo[vsid][cText]) > 3) { vip = " {E3DE31}[V]"; }
			else if(strlen(CarInfo[vsid][cText]) == 0) { vip = ""; }
			if(CarInfo[vsid][cRainBow]) { rainbow = " {791DD5}[R]"; }
			else rainbow = "";
			if(CarInfo[vsid][cStage] > 0) format(stage, sizeof(stage), " {509AE4}[S: %d/3]", CarInfo[vsid][cStage]);
			else if(CarInfo[vsid][cStage] == 0) { stage = ""; }

			if(CarInfo[vsid][cStage] > 0 || CarInfo[vsid][cRainBow] || CarInfo[vsid][cLegendary] == 1 || strlen(CarInfo[vsid][cText]) > 3) 
				format(Other, sizeof(Other), "%s%s%s%s", Legendary, vip, rainbow, stage);
			else {Other = "None";}

			if(CarInfo[vsid][cLock] > 0) {locktext = "{F92E2E}Locked{A8C7DE}";}
			else { locktext = "{47F564}Unlocked{A8C7DE}"; }
			format(string, sizeof(string), "{A8C7DE}SQLID: %d\nVehicle name: %s (%d)\nVehcile Status: %s\nColors: {%s}%d{A8C7DE}, {%s}%d\n{A8C7DE}Vehicle odometer: %.2f km\nInsurance points: %d/10\nInsurance tax: $%s per point\nDays: %d\nOther detalis: %s", 
			CarInfo[vsid][cID], aVehicleNames[CarInfo[vsid][cModel]-400], CarInfo[vsid][Spawned], locktext, GetVehicleColors[Culoare1], Culoare1, GetVehicleColors[Culoare2], Culoare2, CarInfo[vsid][KM], CarInfo[vsid][cPoints], FormatNumber(CarInfo[vsid][cTax]), CarInfo[vsid][Dayss], Other);
			Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Vehicle info", string, "Close", "");	
		}
		case 1: {
			format(string, sizeof(string), "1. Premium: %s\n2. Stage: %d/3\n3. Vip: %s\n4. RainBow: %s\n5. Days: %d\n6. Fuel: %d\n7. Color 1: %d\n8. Color 2: %d\n9. Odometer: %.2f\n10. Temporar: %s (%d Days)\n11. {FF0000}Delete Vehicle", CarInfo[vsid][cLegendary] == 0 ? ("No") : ("Yes"), 
			CarInfo[vsid][cStage], strlen(CarInfo[vsid][cText]) > 3 ? ("Yes") : ("No"), !CarInfo[vsid][cRainBow] ? ("No") : ("Yes"), CarInfo[vsid][Dayss], CarInfo[vsid][cFuel], CarInfo[vsid][cColorOne], CarInfo[vsid][cColorTwo], CarInfo[vsid][KM], CarInfo[vsid][cTemporar] != 1 ? ("Nu") : ("Da"), GetTimeEx(CarInfo[vsid][cTemporarTime]));
			
			Dialog_Show(playerid, DIALOG_EDITCARS11, DIALOG_STYLE_LIST, "Edit Others", string, "Select", "Cancel");
		}
	}
	return 1;
}
Dialog:DIALOG_EDITCARS11(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	switch(listitem) {
		case 0: Dialog_Show(playerid, DIALOG_EDITCARS12, DIALOG_STYLE_INPUT, "Edit premium:", "Edit premium: (0 = No), (1 = Yes).", "Select", "Cancel");
		case 1: Dialog_Show(playerid, DIALOG_EDITCARS12, DIALOG_STYLE_INPUT, "Edit stage:", "Edit stage:", "Select", "Cancel");
		case 2: Dialog_Show(playerid, DIALOG_EDITCARS12, DIALOG_STYLE_INPUT, "Edit vip:", "Edit vip: (0 = No), (1 = Yes).", "Select", "Cancel");
		case 3: Dialog_Show(playerid, DIALOG_EDITCARS12, DIALOG_STYLE_INPUT, "Edit rainbow:", "Edit rainbow: (0 = No), (1 = Yes).", "Select", "Cancel");
		case 4: Dialog_Show(playerid, DIALOG_EDITCARS12, DIALOG_STYLE_INPUT, "Edit Days:", "Edit Days:\nSet your days", "Select", "Cancel");
		case 5: Dialog_Show(playerid, DIALOG_EDITCARS12, DIALOG_STYLE_INPUT, "Edit Fuel:", "Edit Fuel:\nSet your Fuel", "Select", "Cancel");	
		case 6: Dialog_Show(playerid, DIALOG_EDITCARS12, DIALOG_STYLE_INPUT, "Edit Color 1:", "Edit Color 1:\nEnter number of colors", "Select", "Cancel");			
		case 7: Dialog_Show(playerid, DIALOG_EDITCARS12, DIALOG_STYLE_INPUT, "Edit Color 2:", "Edit Color 2:\nEnter number of colors", "Select", "Cancel");			
		case 8: Dialog_Show(playerid, DIALOG_EDITCARS12, DIALOG_STYLE_INPUT, "Edit Odometer:", "Edit Odometer:\nEnter number of odometer", "Select", "Cancel");
		case 9: Dialog_Show(playerid, DIALOG_EDITCARS12, DIALOG_STYLE_INPUT, "Edit temporar:", "Edit Temporar:\nEnter number of temporar", "Select", "Cancel");
		case 10: Dialog_Show(playerid, DIALOG_EDITCARS12, DIALOG_STYLE_MSGBOX, "Delete Vehicle:", "Esti sigur ca vrei sa stergi acest vehicul?", "Ok", "Cancel");		
		case 11: Dialog_Show(playerid, DIALOG_EDITCARS12, DIALOG_STYLE_INPUT, "Change Owner:", "Introdu id-ul:", "Ok", "Cancel");						
	}
	listitemm[playerid] = listitem;
	return 1;
}
Dialog:DIALOG_EDITCARS12(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new vsid = EditVehicleID[playerid];
	new amount = strval(inputtext);
	new query[250];
	switch(listitemm[playerid]) {
		case 0: {
			if(CarInfo[vsid][cModel] == 411 || CarInfo[vsid][cModel] == 522) {
				if(amount > 1) return SCM(playerid, COLOR_GREY, "Invalid!");
				CarInfo[vsid][cLegendary] = amount;
				mysql_format(SQL, query, sizeof(query), "UPDATE cars SET `Legendary`='%d' WHERE `ID`='%d'", CarInfo[vsid][cLegendary], CarInfo[vsid][cID]);
				mysql_tquery(SQL, query, "", "");
			}
			else return SCM(playerid, COLOR_GREY, "Nu poti edit acest vehicul.");
		}
		case 1: {
			if(amount > 3) return SCM(playerid, COLOR_GREY, "Invalid!");
			CarInfo[vsid][cStage] = amount;
			mysql_format(SQL, query, sizeof(query), "UPDATE cars SET `Stage`='%d' WHERE `ID`='%d'", CarInfo[vsid][cStage], CarInfo[vsid][cID]);
			mysql_tquery(SQL, query, "", "");
		}
		case 2: {
			if(valid_vip_vehicle(CarInfo[vsid][cModel]) == 0) return SCM(playerid, COLOR_GREY, "Nu poti edit acest vehicul.");
			if(amount > 1) return SCM(playerid, COLOR_GREY, "Invalid!");
			if(amount == 1) format(CarInfo[vsid][cText], 32, GetName(CarEID[playerid]));
			else format(CarInfo[vsid][cText], 32, "");
			mysql_format(SQL, query, sizeof(query), "UPDATE cars SET `Text`='%d' WHERE `ID`='%d'", CarInfo[vsid][cText], CarInfo[vsid][cID]);
			mysql_tquery(SQL, query, "", "");
			attach_vip_text(vsid);		
		}
		case 3: {
			if(amount > 1) return SCM(playerid, COLOR_GREY, "Invalid!");
			CarInfo[vsid][cRainBow] = amount ? true : false;
			mysql_format(SQL, query, sizeof(query), "UPDATE cars SET `RainBow`='%d' WHERE `ID`='%d'", CarInfo[vsid][cRainBow], CarInfo[vsid][cID]);
			mysql_tquery(SQL, query, "", "");
		}
		case 4: {
			CarInfo[vsid][Dayss] = amount;
			mysql_format(SQL, query, sizeof(query), "UPDATE cars SET `Days`='%d' WHERE `ID`='%d'", CarInfo[vsid][Dayss], CarInfo[vsid][cID]);
			mysql_tquery(SQL, query, "", "");
		}
		case 5: {
			CarInfo[vsid][cFuel] = amount;
			mysql_format(SQL, query, sizeof(query), "UPDATE cars SET `Fuel`='%d' WHERE `ID`='%d'", CarInfo[vsid][cFuel], CarInfo[vsid][cID]);
			mysql_tquery(SQL, query, "", "");
		}
		case 6: {
			if(amount > 255) return SCM(playerid, COLOR_GREY, "Invalid!");
			CarInfo[vsid][cColorOne] = amount;
			mysql_format(SQL, query, sizeof(query), "UPDATE cars SET `ColorOne`='%d' WHERE `ID`='%d'", CarInfo[vsid][cColorOne], CarInfo[vsid][cID]);
			mysql_tquery(SQL, query, "", "");
		}
		case 7: {
			if(amount > 255) return SCM(playerid, COLOR_GREY, "Invalid!");
			CarInfo[vsid][cColorTwo] = amount;
			mysql_format(SQL, query, sizeof(query), "UPDATE cars SET `ColorTwo`='%d' WHERE `ID`='%d'", CarInfo[vsid][cColorTwo], CarInfo[vsid][cID]);
			mysql_tquery(SQL, query, "", "");
		}
		case 8: {
			CarInfo[vsid][KM] = amount;
			mysql_format(SQL, query, sizeof(query), "UPDATE cars SET `KM`='%f' WHERE `ID`='%d'", CarInfo[vsid][KM], CarInfo[vsid][cID]);
			mysql_tquery(SQL, query, "", "");
		}
		case 9: {
			if(amount > 0) {
				CarInfo[vsid][cTemporarTime] = GetTimeNew(amount);
				CarInfo[vsid][cTemporar] = 1;
				mysql_format(SQL, query, sizeof(query), "UPDATE cars SET `Temporar`='1', `TemporarTime`='%d' WHERE `ID`='%d'", CarInfo[vsid][cTemporarTime], CarInfo[vsid][cID]);
				mysql_tquery(SQL, query, "", "");
			}
			else {
				CarInfo[vsid][cTemporarTime] = 0;
				CarInfo[vsid][cTemporar] = 0;
				mysql_format(SQL, query, sizeof(query), "UPDATE cars SET `Temporar`='0', `TemporarTime`='0' WHERE `ID`='%d'", CarInfo[vsid][cID]);
				mysql_tquery(SQL, query, "", "");
			}
		}
		case 10: {
			if(CarInfo[vsid][Spawned] != 0) DestroyPlayerCar(vsid);
			Iter_Remove(MyVehicle[CarEID[playerid]], vsid);
			mysql_format(SQL, query, sizeof(query), "DELETE FROM `cars` WHERE `ID`='%d' LIMIT 1", CarInfo[vsid][cID]);
			mysql_tquery(SQL, query, "", "");	
		}
	}
	SCM(playerid, COLOR_LGREEN, "(/checkv) Modifed sucess.");
	return 1;
}
Dialog:DIALOG_GARAGE1(playerid, response, listitem, inputtext[]) {
	if(!response) return ShowPlayerVehicle(playerid, false);
	new vsid = listitemm[playerid];
	new string[144], query[200];
	switch(listitem) {
		case 0: ShowCarInfo(playerid, vsid);
		case 1: { 
			if(CarInfo[vsid][Spawned] == 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau nu este spawnat!");
			if(CP[playerid] != 0) return Dialog_Show(playerid, DIALOG_CHECKPOINT, DIALOG_STYLE_MSGBOX, "Checkpoint", "Ai deja un checkpoint activ.\nDoresti sa-l anulezi? Daca da, apasa pe 'Ok'.", "Ok", "Exit");
			new Float:vehx, Float:vehy, Float:vehz;
			GetVehiclePos(CarInfo[vsid][Spawned], vehx, vehy, vehz);
			SetPlayerCheckpointEx(playerid, vehx, vehy, vehz, 3.0);
			CP[playerid] = 53;
		}
		case 2: {			
			if(PlayerMoney(playerid, 1000)) return SCM(playerid, COLOR_GREY, "Nu ai $1000!");
			if(CarInfo[vsid][Spawned] == 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau nu este spawnat!");
			if(IsVehicleOccupied(CarInfo[vsid][Spawned])) return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau este ocupat!");
			if(CarInfo[vsid][Confiscated] == 1) return SCM(playerid, -1, "Acest vehicul este confiscat deoarece a fost parcat neregulamentar! Plateste amenda (ultimul rand din meniu).");
			if(IsAttached(CarInfo[vsid][Spawned])) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu poti folosi aceasta functie momentan!");
			SetVehicleToRespawn(CarInfo[vsid][Spawned]);
			SetVehicleHealth(CarInfo[vsid][Spawned], CarInfo[vsid][cVip] ? 2000 : 1000);
			SetVehicleVirtualWorld(CarInfo[vsid][Spawned], CarInfo[vsid][cVirtual]);
			CarInfo[vsid][cTime] = gettime()+600;
			SetTunning(vsid);
			GivePlayerCash(playerid, 0, 1000);
			BizzInfo[19][bBalance] += 1000;
			mysql_format(SQL, string, sizeof(string), "UPDATE bizz SET Till = %d WHERE ID = %d",BizzInfo[19][bBalance],30);
			mysql_tquery(SQL, string, "", "");
			format(gString, 65, "Your %s has been respawned.", aVehicleNames[CarInfo[vsid][cModel]-400]);
			SCM(playerid, COLOR_LIGHTBLUE, gString);
		}			
		case 3: {
			if(CarInfo[vsid][Spawned] == 0) {	
				if(CarInfo[vsid][Confiscated] == 1) return SCM(playerid, -1, "Acest vehicul este confiscat deoarece a fost parcat neregulamentar! Plateste amenda (ultimul rand din meniu).");	
				if(MyVehicleSpawn[playerid] == 5) return SCM(playerid, COLOR_GREY, "Ai deja 5 masini spawnate.");
				SpawnPlayerCar(vsid);
				new Float: x, Float: y, Float: z, szZone[30];
				GetVehiclePos(CarInfo[vsid][Spawned], x, y, z);
				GetPlayer3DZone2(x, y, z, szZone, sizeof(szZone));
				format(gString, 130, "you have spawned your %s which is located in %s. /findcar to track it.", aVehicleNames[CarInfo[vsid][cModel]-400], szZone);
				SCM(playerid, COLOR_LIGHTBLUE, gString);						
			}
			else {
				if(IsAttached(CarInfo[vsid][Spawned])) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu poti folosi aceasta functie momentan!");
				if(IsVehicleOccupied(CarInfo[vsid][Spawned])) return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau este ocupat!");
				DestroyPlayerCar(vsid);			
				format(gString, 65, "Your %s has been despawned.", aVehicleNames[CarInfo[vsid][cModel]-400]);
				SCM(playerid, COLOR_LIGHTBLUE, gString);
			}
		}
		case 4: {
			if(CarInfo[vsid][Spawned] == 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau nu este spawnat!");
			if(IsAttached(CarInfo[vsid][Spawned])) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu poti folosi aceasta functie momentan!");
			if(IsVehicleOccupied(CarInfo[vsid][Spawned])) return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau este ocupat!");
			if(CP[playerid] != 0) return Dialog_Show(playerid, DIALOG_CHECKPOINT, DIALOG_STYLE_MSGBOX, "Checkpoint", "Ai deja un checkpoint activ.\nDoresti sa-l anulezi? Daca da, apasa pe 'Ok'.", "Ok", "Exit");
			SetVehiclePosEx(playerid, CarInfo[vsid][Spawned], 1688.9471,1306.3896,10.9159);
			SetVehicleZAngleEx(CarInfo[vsid][Spawned], 359.3103);
			CP[playerid] = 53;
			if(IsAFLYCar(CarInfo[vsid][Spawned])) {
				SCM(playerid, COLOR_GREY, "Follow the checkpoint on the map to get to your vehicle");
				SetVehiclePosEx(playerid, CarInfo[vsid][Spawned], -1649.1798, -159.0667, 17.3618);
				SetVehicleZAngleEx(CarInfo[vsid][Spawned], -44.1000);
				new randspawn = random(sizeof(HeliVehPos));
				SetVehiclePosEx(playerid, CarInfo[vsid][Spawned], HeliVehPos[randspawn][0], HeliVehPos[randspawn][1], HeliVehPos[randspawn][2]);
				SetVehicleZAngleEx(CarInfo[vsid][Spawned], 44.4000);
				SetPlayerCheckpointEx(playerid, HeliVehPos[randspawn][0], HeliVehPos[randspawn][1], HeliVehPos[randspawn][2], 5.0);
				return true;
			}
			else if(IsABOATCar(CarInfo[vsid][Spawned])) {
				SCM(playerid, COLOR_GREY, "Follow the checkpoint on the map to get to your vehicle");
				new randspawn = random(sizeof(BoatVehPos));
				SetVehiclePosEx(playerid, CarInfo[vsid][Spawned], BoatVehPos[randspawn][0], BoatVehPos[randspawn][1], BoatVehPos[randspawn][2]);
				SetVehicleZAngleEx(CarInfo[vsid][Spawned], -173.4001);
				SetPlayerCheckpointEx(playerid, BoatVehPos[randspawn][0], BoatVehPos[randspawn][1], BoatVehPos[randspawn][2], 5.0);
				return true;
			}
			else {
				SCM(playerid, COLOR_GREY, "Follow the checkpoint on the map to get to your vehicle");
				new randspawn = random(sizeof(DealerCarPos));
				SetVehiclePosEx(playerid, CarInfo[vsid][Spawned], DealerCarPos[randspawn][0], DealerCarPos[randspawn][1], DealerCarPos[randspawn][2]);
				SetVehicleZAngleEx(CarInfo[vsid][Spawned], -173.4001);
				SetPlayerCheckpointEx(playerid, DealerCarPos[randspawn][0], DealerCarPos[randspawn][1], DealerCarPos[randspawn][2], 5.0);
				return true;
			}
		}
		case 5: {
			if(TradeID[playerid] != -1) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu poti folosi aceasta comanda atata timp cat esti implicat intr-o afacere!");
			if(CarInfo[vsid][Spawned] == 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau nu este spawnat!");
			if(!PlayerToPoint(5,playerid, 1487.0498,-2287.2930,13.7529)) return SCM(playerid, COLOR_WHITE, "Trebuie sa fii la dealership. Foloseste /gps pentru a afla unde este.");
			if(IsAttached(CarInfo[vsid][Spawned])) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu poti folosi aceasta functie momentan!");
			new value;
			if(CarInfo[vsid][cSpecial] != 1) value = CarInfo[vsid][cValue] / 2;
			else value = CarInfo[vsid][cValue];
			format(string, sizeof(string), "You're sure you want to sell your %s for %s?",aVehicleNames[CarInfo[vsid][cModel]-400],FormatNumber(value));
			Dialog_Show(playerid, DIALOG_SELLCAR, DIALOG_STYLE_MSGBOX, "Dealership", string, "Select", "Cancel");
			PlayerSellCar[playerid] = vsid;
			if(CP[playerid] == 43 || CP[playerid] == 53) { CP[playerid] = 0; DisablePlayerCheckpointEx(playerid); }
		}
		case 6: Dialog_Show(playerid, DIALOG_INSURANCE, DIALOG_STYLE_INPUT, "Insurance", "Write below how many insurance points you want to buy. \nYou have a maximum of 10 points and the price is based on the car's taxes.", "Buy", "Cancel");		
		case 7: {
			if(CarInfo[vsid][Spawned] == 0) 
				return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau nu este spawnat!");

			if(strlen(CarInfo[vsid][cText]) > 3) 
				return SCM(playerid, -1, "Acest vehicul este deja unul VIP!");
			
			if(PlayerInfo[playerid][pPremiumPoints] < 300) 
				return SCM(playerid, -1, "Nu ai destule Premium Points.");
			
			SetPVarInt(playerid, "VehicleID", vsid);
			Dialog_Show(playerid, DIALOG_UPGRADEVIP, DIALOG_STYLE_MSGBOX, "Upgrade to VIP Vehicle", "Are you sure you want to do this? \nIt lose 300 Premium Points.", "Upgrade", "Cancel");
		}
		case 8: {
			switch(CarInfo[vsid][cVip]) {
				case 0: {
					if(CarInfo[vsid][cVip])
						return SCM(playerid, COLOR_LGREEN, "Already upgraded to VIP PLUS.");

					if(strlen(CarInfo[vsid][cText]) < 3) 
						return SCM(playerid, -1, "Eroare: Trebuie sa fie upgradat la vip.");

					if(PlayerInfo[playerid][pPremiumPoints] < 500) 
						return SCM(playerid, -1, "Nu ai destule Premium Points.");

					SetPVarInt(playerid, "VehicleID", vsid);
					Dialog_Show(playerid, DIALOG_UPGRADEVIPL, DIALOG_STYLE_MSGBOX, "Upgrade to VIP Plus Vehicle", "Esti sigur ca vrei sa faci acest upgrade? (500PP)\n\nAvantaje:\n-Vehiculul are 2000HP.\n-Posibilitatea de a aduce vehiculul la tine odata pe ora.", "Upgrade", "Cancel");
				}
				case 1: {
					if(CarInfo[vsid][Spawned] == 0) 
						return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau nu este spawnat!");

					if(IsVehicleOccupied(CarInfo[vsid][Spawned])) 
						return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau este ocupat!");

					if(gettime() < CarInfo[vsid][cTimeGoto])
						return SCMf(playerid, COLOR_GREY, "Poti aduce masina la tine peste %s minute.", CalculateTime(CarInfo[vsid][cTimeGoto]-gettime()));
					
					new Float: X, Float: Y, Float: Z;
					GetPlayerPos(playerid, X, Y, Z);
					SetVehiclePosEx(playerid, CarInfo[vsid][Spawned], X, Y+4, Z);
					SCMf(playerid, COLOR_LGREEN, "(Info) {FFFFFF}You have successfully brought the %s vehicle to you", aVehicleNames[CarInfo[vsid][cModel]-400]);
					CarInfo[vsid][cTimeGoto] = gettime()+3600;
					gQuery[0] = EOS;
					mysql_format(SQL, gQuery, 500, "UPDATE `cars` SET `TimeGoto`='%d' WHERE `ID`='%d'", CarInfo[vsid][cTimeGoto], CarInfo[vsid][cID]);
					mysql_tquery(SQL, gQuery, "", "");
				}
			}
		}
		case 9: {
			if(CarInfo[vsid][Spawned] == 0) 
				return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau nu este spawnat!");

			if(CarInfo[vsid][cRainBow]) 
				return SCM(playerid, -1, "Acest vehicul este deja unul RainBow!");

			if(PlayerInfo[playerid][pPremiumPoints] < 300) 
				return SCM(playerid, -1, "Nu ai destule Premium Points.");

			SetPVarInt(playerid, "VehicleID", vsid);
			Dialog_Show(playerid, DIALOG_UPGRADERAINBOW, DIALOG_STYLE_MSGBOX, "Upgrade to RainBow", "Are you sure you want to do this? \nIt lose 300 Premium Points.", "Upgrade", "Cancel");
		}
		case 10: {
			if(CarInfo[vsid][Spawned] == 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau nu este spawnat!");
			new car = CarInfo[vsid][Spawned];
			for(new i = 0; i < 14; i++) {
				if(CarInfo[vsid][cMod][i] != 0) RemoveVehicleComponent(car, CarInfo[vsid][cMod][i]);
				CarInfo[vsid][cMod][i] = 0;
			}
			CarInfo[vsid][paintjob] = 3;
			ChangeVehiclePaintjob(CarInfo[vsid][Spawned], 3);
			update_mods(vsid);
			mysql_format(SQL, query, sizeof(query), "UPDATE `cars` SET `paintjob`='%d' WHERE `ID`='%d'",CarInfo[vsid][paintjob], CarInfo[vsid][cID]);
			mysql_tquery(SQL, query, "", "");					
		}
		case 11: {
			if(CarInfo[vsid][Confiscated] == 0) return true;
			if(PlayerMoney(playerid, CarInfo[vsid][cTax])) return SCM(playerid, -1, "You do not have the money you need!");
			Dialog_Show(playerid, DIALOG_TAX, DIALOG_STYLE_MSGBOX, "He pays the fine",
			"You're sure you want to pay the fine for unregulated parking?.", "Select", "Cancel");
		}				
	}					
	return 1;
}
Dialog:DIALOG_UPGRADEVIP(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new idd = GetPVarInt(playerid, "VehicleID");
	if(CarInfo[idd][Spawned] == 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau nu este spawnat!");
	//if(valid_vip_vehicle(CarInfo[idd][cModel]) == 0) return SCM(playerid, COLOR_GREY, "Acest vehicul nu poate fi upgradat!");
	if(GetPlayerVehicleID(playerid) != CarInfo[idd][Spawned]) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu te afli in acel vehicul!");
	if(strlen(CarInfo[idd][cText]) > 3) return SCM(playerid, -1, "Acest vehicul este deja unul VIP!");
	if(PlayerInfo[playerid][pPremiumPoints] < 300) return SCM(playerid, -1, "Nu ai destule Premium Points.");
	if(IsABike2(CarInfo[idd][Spawned])) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu poti upgrada acest vehicul!");
	PlayerInfo[playerid][pPremiumPoints] -= 300;
	Update(playerid, pPremiumPointsx);	
	new string[144];	
	SCM(playerid, COLOR_YELLOW, "* Felicitari! Ti-ai upgradat cu succes vehiculul la unul VIP.");
	format(string, sizeof(string), "%s si-a upgradat vehiculul %s[id: %d] la unul VIP pentru 300 Premium Points.", GetName(playerid), aVehicleNames[CarInfo[idd][cModel]-400], idd);
	InsertLog(playerid, string, LOG_SHOP);				
	mysql_format(SQL, string, sizeof(string), "UPDATE `cars` SET `Text`='%s' WHERE `ID`='%d'", GetName(playerid), CarInfo[idd][cID]);
	mysql_tquery(SQL, string, "", "");
	format(CarInfo[idd][cText], 32, GetName(playerid));
	EditVipCar(playerid, idd);
	return 1;
}
Dialog:DIALOG_UPGRADEVIPL(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new idd = GetPVarInt(playerid, "VehicleID");
	if(CarInfo[idd][cVip] != 0) 
		return SCM(playerid, COLOR_LGREEN, "Eroare: Ai deja upgradat la VIP Plus.");

	if(CarInfo[idd][Spawned] == 0) 
		return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau nu este spawnat!");

	if(strlen(CarInfo[idd][cText]) < 3) 
		return SCM(playerid, -1, "Eraore: Acest vehicul nu are VIP.");

	if(PlayerInfo[playerid][pPremiumPoints] < 500) 
		return SCM(playerid, -1, "Nu ai destule Premium Points.");

	PlayerInfo[playerid][pPremiumPoints] -= 500;
	Update(playerid, pPremiumPointsx);	
	new string[144];	
	SCM(playerid, COLOR_YELLOW, "* Felicitari! Ti-ai upgradat cu succes vehiculul la unul VIP Plus.");
	format(string, sizeof(string), "%s si-a upgradat vehiculul %s[id: %d] la unul VIP Plus pentru 500 Premium Points.", GetName(playerid), aVehicleNames[CarInfo[idd][cModel]-400], idd);
	InsertLog(playerid, string, LOG_SHOP);		
	CarInfo[idd][cVip] = 1;	
	CarInfo[idd][cTimeGoto] = gettime();	
	gQuery[0] = EOS;
	mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `cars` SET `VIP`='%d', `TimeGoto`='%d' WHERE `ID`='%d'", CarInfo[idd][cVip], CarInfo[idd][cTimeGoto], CarInfo[idd][cID]);
	mysql_tquery(SQL, gQuery, "", "");
	return 1;
}
Dialog:DIALOG_UPGRADERAINBOW(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new idd = GetPVarInt(playerid, "VehicleID");
	if(CarInfo[idd][Spawned] == 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau nu este spawnat!");
	if(CarInfo[idd][cRainBow]) return SCM(playerid, -1, "Acest vehicul este deja unul RainBow!");
	if(PlayerInfo[playerid][pPremiumPoints] < 300) return SCM(playerid, -1, "Nu ai destule Premium Points.");
	PlayerInfo[playerid][pPremiumPoints] -= 300;
	Update(playerid, pPremiumPointsx);
	new string[144];	
	SCM(playerid, COLOR_YELLOW, "* Felicitari! Ti-ai upgradat cu succes vehiculul la unul RainBow.");
	format(string, sizeof(string), "%s si-a upgradat vehiculul %s[id: %d] la unul RainBow pentru 200 Premium Points.", GetName(playerid), aVehicleNames[CarInfo[idd][cModel]-400], idd);
	InsertLog(playerid, string, LOG_SHOP);
	CarInfo[idd][cRainBow] = true;
	mysql_format(SQL, string, sizeof(string), "UPDATE `cars` SET `RainBow`='%d' WHERE `ID`='%d'", CarInfo[idd][cRainBow], CarInfo[idd][cID]);
	mysql_tquery(SQL, string, "", "");
	return 1;
}
Dialog:DIALOG_UPSTAGE(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new idd = PersonalCar(playerid);
	if(CarInfo[idd][cStage] >= 3) return SCM(playerid, -1, "Acest vehicul este upgradat la stage maximum!");
	if(PlayerInfo[playerid][pPremiumPoints] < StagePrice[0][CarInfo[idd][cStage]+1]) return SCM(playerid, -1, "Nu ai destule Premium Points.");
	if(IsABike(CarInfo[idd][Spawned])) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu poti upgrada acest vehicul!");
	PlayerInfo[playerid][pPremiumPoints] -= StagePrice[0][CarInfo[idd][cStage]+1];
	Iter_Add(IsSpeed, playerid);
	Update(playerid, pPremiumPointsx);
	new string[144];	
	SCMEx(playerid, COLOR_WHITE, "{227FC3}Tunning System: {FFFFFF}You upgrade your vehicle successfully to stage %d for %d premium points.", CarInfo[idd][cStage]+1, StagePrice[0][CarInfo[idd][cStage]+1]);
	mysql_format(SQL, string, sizeof(string), "UPDATE `cars` SET `Stage`='%d' WHERE `ID`='%d'", CarInfo[idd][cStage]+1, CarInfo[idd][cID]);
	mysql_tquery(SQL, string, "", "");
	CarInfo[idd][cStage] ++;
	format(string, sizeof(string), "Your vehicle is~y~ %s~n~~w~~h~Max. speed:~y~ %d km/h~n~~w~~h~Current stage:~y~ %d/3~w~~h~~n~boost: ~y~+%d km/h~w~~h~ / ~b~ %d km/h", aVehicleNames[GetVehicleModel(GetPlayerVehicleID(playerid))-400], CarInfo[idd][cMaxSpeed], CarInfo[idd][cStage],StageSpeed[0][CarInfo[idd][cStage]], CarInfo[idd][cMaxSpeed]+StageSpeed[0][CarInfo[idd][cStage]]);
	PlayerTextDrawSetString(playerid, StagePTD, string);

	return 1;
}
Dialog:DIALOG_TAX(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new vsid = listitemm[playerid], query[200];
	GivePlayerCash(playerid, 0, CarInfo[vsid][cTax]);
	CarInfo[vsid][Confiscated] = 0;

	mysql_format(SQL, query, sizeof(query), "UPDATE `cars` SET `Confiscated`='0' WHERE `ID`='%d'", CarInfo[vsid][cID]);
	mysql_tquery(SQL, query, "", "");
	new string[144];
	format(string, sizeof(string), "**(( TTC Dispatch: %s (%d) si-a platit amenda in valoare de $%s pentru a-si recupera vehiculul %d ))**", GetName(playerid), playerid, FormatNumber(CarInfo[vsid][cTax]), vsid);
	SendFactionMessage(8, COLOR_LIGHTBLUE, string);
	format(string, sizeof(string), "Ai platit amenda de $%s pentru parcarea neregulamentara. Ai grija data viitoare!", FormatNumber(CarInfo[vsid][cTax]));
	SCM(playerid, COLOR_MONEY, string);
	DynamicFactions[8][fBank] += CarInfo[vsid][cTax]; 
	return 1;
}
Dialog:DIALOG_RAINBOW1(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new idd = GetPVarInt(playerid, "VehicleIDD"), query[200];
	if(CarInfo[idd][Spawned] == 0) return SCM(playerid, COLOR_LGREEN, "Eroare: Vehiculul tau nu este spawnat!");
	if(CarInfo[idd][cRainBow]) return SCM(playerid, -1, "Acest vehicul este deja unul RainBow!");
	if(PlayerInfo[playerid][pPremiumPoints] < 200) return SCM(playerid, -1, "Nu ai destule Premium Points.");
	PlayerInfo[playerid][pPremiumPoints] -= 200;
	Update(playerid, pPremiumPointsx);
	CarInfo[idd][cRainBow] = true;
	SCM(playerid, COLOR_YELLOW, "* Felicitari! Ti-ai upgradat cu succes vehiculul la unul RainBow.");
	mysql_format(SQL, query, sizeof(query), "UPDATE `cars` SET `RainBow`='1' WHERE `ID`='%d'", CarInfo[idd][cID]);
	mysql_tquery(SQL, query, "", "");
	return 1;
}
Dialog:DIALOG_INSURANCE(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new vsid = listitemm[playerid];
	new points = strval(inputtext);
	new query[200];
	if(points < 1) return true;
	if(points + CarInfo[vsid][cPoints] > 10) return SCM(playerid, COLOR_GREY, "Nu poti avea mai mult de 10 puncte de asigurare!");
	CarInfo[vsid][cPoints] += points;
	new string[144];	
	format(string, sizeof(string), "(+) Ai cumparat %d puncte de asigurare pentru $%s.", points, FormatNumber(1000*points));
	SCM(playerid, COLOR_MONEY, string);
	GivePlayerCash(playerid, 0, points*1000);
	mysql_format(SQL, query, sizeof(query), "UPDATE `cars` SET `Points`='%d' WHERE `ID`='%d'", CarInfo[vsid][cPoints], CarInfo[vsid][cID]);
	mysql_tquery(SQL, query, "", "");
	return 1;
}
Dialog:DIALOG_CLANTURFS(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(CP[playerid] != 0) return Dialog_Show(playerid, DIALOG_CHECKPOINT, DIALOG_STYLE_MSGBOX, "Checkpoint", "Ai deja un checkpoint activ.\nDoresti sa-l anulezi? Daca da, apasa pe 'Ok'.", "Ok", "Exit");
	new i;
	i = listitem+1;
	SetPlayerCheckpointEx(playerid, GraffitiInfo[i][gfX], GraffitiInfo[i][gfY], GraffitiInfo[i][gfZ]-3, 5.0);
	SCM(playerid, COLOR_YELLOW, "Server-ul ti-a pus un punct rosu catre locatia aleasa de tine.");
	CP[playerid] = 53;
	return 1;
}
Dialog:DIALOG_CLAN_COMMAND(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new string[144];
	switch(listitem) {
		case 0: Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX , "Comenzile clanului:", "/clan, /(c)lanchat, /cinvite, /clanduty, /spray, /clanzones.", "Close", "");
		case 1: {
			new
				szDialog[1024], szDialog2[1024],
				Members, name[180], cwarn[180], cdays[180],
				szRank1[180], query[250];

			format(query, sizeof(query), "SELECT * FROM `users` WHERE `users`.`Clan` = '%d' ORDER BY `users`.`ClanRank` DESC LIMIT 50", PlayerInfo[playerid][pClan]);
			new Cache: result = mysql_query(SQL, query);
	
			strcat(szDialog2, "#. Name\tRank\tClan Warns\tClan Days\n");
			for(new i, j = cache_num_rows(); i != j; ++i)
			{
				cache_get_value_name(i, "name", name);
				cache_get_value_name(i, "ClanRank", szRank1);
				cache_get_value_name(i, "ClanWarns", cwarn);
				cache_get_value_name(i, "ClanDays", cdays);
				
				format(Selected[playerid][Members], MAX_PLAYER_NAME, name);
				format(szDialog, sizeof(szDialog), "%d. %s\t%d\t%s/3\t%s\n", Members+1, name, strval(szRank1), cwarn, cdays);
				strcat(szDialog2, szDialog);
				Members++;
			}
			cache_delete(result);
			Dialog_Show(playerid, DIALOG_CLAN_MEMBERS, DIALOG_STYLE_TABLIST_HEADERS, "Clan members", szDialog2, "Ok", "Back");
		}
		case 2: {
			if(PlayerInfo[playerid][pClanRank] < 6) return true;
			Dialog_Show(playerid, DIALOG_CLAN_SETTINGS, DIALOG_STYLE_LIST, "Setari:", "Schimba culoarea\nSchimba MOTD-ul\nSchimba numele rankurilor\nSchimba culorarea rankurilor\n{FFCC00}(40 PP){FFFFFF} Schimba numele clanului\n{FFCC00}(40 PP){FFFFFF} Schimba tagul clanului\n{FFCC00}(50PP){FFFFFF} +30 Clan days", "Select", "Back");
		}
		case 3: {
			new clanid = PlayerInfo[playerid][pClan];
			format(string, sizeof(string), "{FFF785}%s{FFFFFF}%s\n%s{FFF785}%s{FFFFFF}\nScoate TAG-ul", ClanInfo[clanid][clTag], GetName(playerid), GetName(playerid), ClanInfo[clanid][clTag]);		
			Dialog_Show(playerid, DIALOG_CLANTAG, DIALOG_STYLE_LIST , "Clan TAG", string, "Ok", "Close");					
		}
		case 4: {
			new clanid = PlayerInfo[playerid][pClan];
			format(string, sizeof(string), "Introdu suma pe care vrei sa o depozitezi in clan\nSuma disponibila: %s", FormatNumbers(ClanInfo[clanid][clPremiumPoints]));
			Dialog_Show(playerid, DIALOG_DEPOSITPP, DIALOG_STYLE_INPUT, "Deposit clan PP", string, "Deposit", "Cancel");
		}
		case 5: {
			new title[40];
			format(title, sizeof(title), "Clan: $%s", FormatNumber(ClanInfo[PlayerInfo[playerid][pClan]][ClanBank]));
			format(string, sizeof(string), "[+] Buy Clan Vehicles");
			Dialog_Show(playerid, DIALOG_CLANVEHICLES, DIALOG_STYLE_LIST, title, string, "Select", "Cancel");
		}	
	}
	return 1;
}
Dialog:DIALOG_DEPOSITPP(playerid, response, listitem, inputtext[]) {
	if(!response) return 1;
	new pp = strval(inputtext), clanid = PlayerInfo[playerid][pClan];
	if(strval(inputtext) <= 0)
		return SCM(playerid, COLOR_GREY, "Invalid Amount");
	new string[144], query[250];
	if(PlayerInfo[playerid][pPremiumPoints] < pp) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu aceste puncte premium!");
	ClanInfo[clanid][clPremiumPoints] += pp;
	PlayerInfo[playerid][pPremiumPoints] -= pp;
	Update(playerid, pPremiumPointsx);
	mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `PremiumPoints`='%d' WHERE `ID`='%d'", pp, clanid);
	mysql_tquery(SQL, query, "", "");
	format(string, sizeof(string), "{FFFF00}[CLAN DEPOSIT] %s a depositat %d Premium Points.", GetName(playerid), pp);
	SendClanMessage(clanid, string);
	return 1;
}
Dialog:DIALOG_CLANVEHICLES(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new title[40];
	new string[144];	
	format(title, sizeof(title), "Clan: $%s", FormatNumber(ClanInfo[PlayerInfo[playerid][pClan]][ClanBank]));
	format(string, sizeof(string), "Infernus - comming\nSavana - comming");
	Dialog_Show(playerid, DIALOG_CLANVEHICLES1, DIALOG_STYLE_LIST, title, string, "Select", "Cancel");
	return 1;
}
Dialog:DIALOG_CLANVEHICLES1(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	switch(listitem) {
		case 0: SCM(playerid, COLOR_RED, "Comming");
		case 1: SCM(playerid, COLOR_RED, "Comming");
	}
	return 1;
}
Dialog:DIALOG_CLANTAG(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new string[144];
	if(listitem == 2) {
		PlayerInfo[playerid][pTag] = -1;
		UpdateVar(playerid, "Tag", -1);
		SCM(playerid, COLOR_LGREEN, "Ti-ai scos tagul!");
		format(string, sizeof(string), "%s", PlayerInfo[playerid][pUsername]);
		SetPlayerName(playerid, string);
		return true;
	}
	PlayerInfo[playerid][pTag] = listitem;
	UpdateVar(playerid, "Tag", listitem);
	SCM(playerid, COLOR_LGREEN, "Pozitia TAG-ului a fost modificata!");
	new clanid = PlayerInfo[playerid][pClan];
	if(PlayerInfo[playerid][pClan] != 0 && PlayerInfo[playerid][pTag] == 0) {
		format(string, sizeof(string), "%s%s", ClanInfo[clanid][clTag], PlayerInfo[playerid][pUsername]);
		SetPlayerName(playerid, string);
	}
	else if(PlayerInfo[playerid][pClan] != 0 && PlayerInfo[playerid][pTag] == 1) {
		format(string, sizeof(string), "%s%s", PlayerInfo[playerid][pUsername], ClanInfo[clanid][clTag]);
		SetPlayerName(playerid, string);
	}		
	return 1;
}	
Dialog:DIALOG_CLAN_MEMBERS(playerid, response, listitem, inputtext[]) {
	if(!response) return Dialog_Show(playerid, DIALOG_CLAN_COMMAND, DIALOG_STYLE_LIST, "Clan:", "Comenzile clanului\nMembrii clanului\nSetari\nTag", "Select", "Close");
	if(PlayerInfo[playerid][pClanRank] < 6) return SCM(playerid, COLOR_WHITE, "La aceasta categorie, are acces doar propietarul clanului.");
	format(ssName[playerid], MAX_PLAYER_NAME, Selected[playerid][listitem]);
	Dialog_Show(playerid, DIALOG_CLAN_MANAGE2, DIALOG_STYLE_LIST, "Setari:", "Uninvite\nGive rank\nClan warn\nUn Clan warn", "Ok" ,"Back");
	return 1;
}
Dialog:DIALOG_CLAN_MANAGE2(playerid, response, listitem, inputtext[]) {
	if(!response) {
		new
			szDialog[1024], szDialog2[1024],
			Members, name[180], cwarn[180], cdays[180],
			szRank1[180], query[250];

		format(query, sizeof(query), "SELECT * FROM `users` WHERE `users`.`Clan` = '%d' ORDER BY `users`.`ClanRank` DESC LIMIT 50", PlayerInfo[playerid][pClan]);
		new Cache: result = mysql_query(SQL, query);

		strcat(szDialog2, "#. Name\tRank\tClan Warns\tClan Days\n");
		for(new i, j = cache_num_rows(); i != j; ++i)
		{
			cache_get_value_name(i, "name", name);
			cache_get_value_name(i, "ClanRank", szRank1);
			cache_get_value_name(i, "ClanWarns", cwarn);
			cache_get_value_name(i, "ClanDays", cdays);
			
			format(Selected[playerid][Members], MAX_PLAYER_NAME, name);
			format(szDialog, sizeof(szDialog), "%d. %s\t%d\t%s/3\t%s\n", Members+1, name, strval(szRank1), cwarn, cdays);
			strcat(szDialog2, szDialog);
			Members++;
		}
		cache_delete(result);
		Dialog_Show(playerid, DIALOG_CLAN_MEMBERS, DIALOG_STYLE_TABLIST_HEADERS, "Clan members", szDialog2, "Ok", "Back");
		return true;
	}
	new string[144], query[250];	
	switch(listitem) {
		case 0: {
			new
				userID = GetPlayerID(ssName[playerid]),
				clanid = PlayerInfo[playerid][pClan];

			if(userID == playerid) return SCM(playerid, COLOR_GREY, "Nu poti folosi aceasta comanda asupra ta!");

			format(string, sizeof(string), "Ai fost dat afara din clanul %s de %s.", ClanInfo[clanid][clName], GetName(playerid));
			InsertEmail(ssName[playerid], GetName(playerid), string, 0);					
			if(userID != INVALID_PLAYER_ID) {
  				format(string, sizeof(string), "Ai fost dat afara din clan de catre %s.", GetName(playerid));
  				SCM(userID, COLOR_LIGHTBLUE, string);
				PlayerInfo[userID][pClan] = 0;
				PlayerInfo[userID][pClanRank] = 0;
				PlayerInfo[userID][pClanDays] = 0;
				PlayerInfo[userID][pClanWarn] = 0;
				format(string, sizeof(string), "{%s}[CLAN] %s l-a dat pe %s afara.", ClanInfo[clanid][clColor], GetName(playerid), ssName[playerid]);
				SetPlayerName(userID, PlayerInfo[userID][pUsername]);
			}
			else format(string, sizeof(string), "{%s}[CLAN] %s l-a dat pe %s (neconectat) afara.", ClanInfo[clanid][clColor], GetName(playerid), ssName[playerid]);
			SendClanMessage(clanid, string);
			ClanMembers[PlayerInfo[playerid][pClan]] --;
			mysql_format(SQL, query, sizeof(query), "UPDATE `users` SET `Clan`='0', `ClanRank`='0', `ClanDays`='0', `ClanWarns`='0' WHERE `Name`='%s'", ssName[playerid]);
			mysql_tquery(SQL, query, "", "");
		}
		case 1:
		{
			format(string, sizeof(string), "Ce rank vrei sa ii acorzi membrului %s?\nScrie un rank valid (1-5).", ssName[playerid]);
			Dialog_Show(playerid, DIALOG_CLAN_GRANK, DIALOG_STYLE_INPUT, "Give rank", string, "Ok", "Back");
		}
		case 2: {
			new userID = GetPlayerID(ssName[playerid]), szQuery[256], clanid = PlayerInfo[playerid][pClan];
			format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `name`='%s'", ssName[playerid]);
			new Cache: result = mysql_query(SQL, szQuery);
			cache_get_value_name(0, "ClanWarns", string);
			new cw = strval(string);					
			cache_delete(result);
			cw ++;
			mysql_format(SQL, query, sizeof(query), "UPDATE `users` SET `ClanWarns`='%d' WHERE `Name`='%s'", cw, ssName[playerid]);
			mysql_tquery(SQL, query, "", "");
			format(string, sizeof(string), "{%s}[CLAN] %s i-a dat lui %s un clan warn.", ClanInfo[clanid][clColor], GetName(playerid), ssName[playerid]);
			SendClanMessage(clanid, string);
			if(userID != INVALID_PLAYER_ID) PlayerInfo[userID][pClanWarn] ++;
			if(cw == 3) {
				ClanMembers[PlayerInfo[playerid][pClan]] --;
				format(string, sizeof(string), "Ai fost dat afara din clanul %s de %s, motiv: 3/3 Clan Warns.", ClanInfo[clanid][clName], GetName(playerid));
				InsertEmail(ssName[playerid], GetName(playerid), string, 0);			
				if(userID != INVALID_PLAYER_ID) {					
					format(string, sizeof(string), "Ai fost dat afara din clan de catre %s (3/3 cw).", GetName(playerid));
					SCM(userID, COLOR_LIGHTBLUE, string);
					PlayerInfo[userID][pClan] = 0;
					PlayerInfo[userID][pClanRank] = 0;
					PlayerInfo[userID][pClanDays] = 0;
					PlayerInfo[userID][pClanWarn] = 0;
					format(string, sizeof(string), "{%s}[CLAN] %s l-a dat pe %s afara (3/3 cw).", ClanInfo[clanid][clColor], GetName(playerid), ssName[playerid]);
					SetPlayerName(userID, PlayerInfo[userID][pUsername]);						
				}
				else format(string, sizeof(string), "{%s}[CLAN] %s l-a dat pe %s (neconectat) afara (3/3 cw).", ClanInfo[clanid][clColor], GetName(playerid), ssName[playerid]);	
				mysql_format(SQL, query, sizeof(query), "UPDATE `users` SET `Clan`='0', `ClanRank`='0', `ClanDays`='0', `ClanWarns`='0' WHERE `Name`='%s'", ssName[playerid]);
				mysql_tquery(SQL, query, "", "");
			}
		}
		case 3: {
			new userID = GetPlayerID(ssName[playerid]), szQuery[256], clanid = PlayerInfo[playerid][pClan];
			format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `name`='%s'", ssName[playerid]);
			new Cache: result = mysql_query(SQL, szQuery);
			cache_get_value_name(0, "ClanWarns", string);
			new cw = strval(string);					
			cache_delete(result);
			if(cw == 0) return SCM(playerid, -1, "Acel membru nu are avertizari!");
			cw --;
			format(string, sizeof(string), "{%s}[CLAN] %s i-a scos lui %s un clan warn.", ClanInfo[clanid][clColor], GetName(playerid), ssName[playerid]);
			SendClanMessage(clanid, string);					
			mysql_format(SQL,query, sizeof(query), "UPDATE `users` SET `ClanWarns`='%d' WHERE `Name`='%s'", cw, ssName[playerid]);
			mysql_tquery(SQL, query, "", "");
			if(userID != INVALID_PLAYER_ID) PlayerInfo[userID][pClanWarn] --;	
		} 
	}
	return 1;
}		
Dialog:DIALOG_CLAN_GRANK(playerid, response, listitem, inputtext[])
{
	if(!response) {
		new
			szDialog[500], szDialog2[500],
			Members, name[110], cwarn[110], cdays[110],
			szRank1[110];
		new query[250];			

		format(query, sizeof(query), "SELECT * FROM `users` WHERE `users`.`Clan` = '%d' ORDER BY `users`.`ClanRank` DESC LIMIT 50", PlayerInfo[playerid][pClan]);
		new Cache: result = mysql_query(SQL, query);

		strcat(szDialog2, "#. Name\tRank\tClan Warns\tClan Days\n");
		for(new i, j = cache_num_rows(); i != j; ++i)
		{
			cache_get_value_name(i, "name", name);
			cache_get_value_name(i, "ClanRank", szRank1);
			cache_get_value_name(i, "ClanWarns", cwarn);
			cache_get_value_name(i, "ClanDays", cdays);
			
			format(Selected[playerid][Members], MAX_PLAYER_NAME, name);
			format(szDialog, sizeof(szDialog), "%d. %s\t%d\t%s/3\t%s\n", Members+1, name, strval(szRank1), cwarn, cdays);
			strcat(szDialog2, szDialog);
			Members++;
		}
		cache_delete(result);
		Dialog_Show(playerid, DIALOG_CLAN_MEMBERS, DIALOG_STYLE_TABLIST_HEADERS, "Clan members", szDialog2, "Ok", "Back");
		return true;
	}
	if(strval(inputtext) <= 0 || strval(inputtext) >= 6) return SCM(playerid, COLOR_GREY, "Rank-ul este invalid. Trebuie sa fie cuprins intre 1 si 5!");
	new
		userID = GetPlayerID(ssName[playerid]),
		clanid = PlayerInfo[playerid][pClan];
	new string[144], query[144];

	if(userID == playerid) return SCM(playerid, COLOR_GREY, "Nu poti folosi aceasta comanda asupra ta!");
	if(userID != INVALID_PLAYER_ID) {
		PlayerInfo[userID][pClanRank] = strval(inputtext);
		format(string, sizeof(string), "{%s}[CLAN] %s i-a dat rank %d lui %s.", ClanInfo[clanid][clColor], GetName(playerid), strval(inputtext), ssName[playerid]);
	}
	else format(string, sizeof(string), "{%s}[CLAN] %s i-a dat rank %d lui %s (neconectat).", ClanInfo[clanid][clColor], GetName(playerid), strval(inputtext), ssName[playerid]);
	SendClanMessage(clanid, string);

	mysql_format(SQL, query, sizeof(query), "UPDATE `users` SET `ClanRank`='%d' WHERE `Name`='%s'", strval(inputtext), ssName[playerid]);
	mysql_tquery(SQL, query, "", "");
	return 1;
}
Dialog:DIALOG_CLAN(playerid, response, listitem, inputtext[]) {
	if(!response) return ShowShop(playerid);
	if(ExistaNumeClan(inputtext)) return SCM(playerid, COLOR_WARNING, "Exista deja un clan inregistrat cu acest nume!");

	if(strlen(inputtext) < 2 || strlen(inputtext) > 20) {
		SCM(playerid, COLOR_GREY, "Numele clanului trebuie sa aiba intre 3 si 20 caractere.");
		if(sSlot[playerid] == 10) {
			Dialog_Show(playerid, DIALOG_CLAN, DIALOG_STYLE_INPUT, "Shop:", "Esti pe cale sa-ti creezi un clan cu 10 locuri disponibile.\nTot ce trebuie sa faci in acest moment,\
			 este sa-ti alegi numele clanului.\nNumele clanului trebuie sa contina intre 3 si 10 caractere.", "Ok", "Back");
		}
		else if(sSlot[playerid] == 20) {
			Dialog_Show(playerid, DIALOG_CLAN, DIALOG_STYLE_INPUT, "Shop:", "Esti pe cale sa-ti creezi un clan cu 20 locuri disponibile.\nTot ce trebuie sa faci in acest moment,\
			 este sa-ti alegi numele clanului.\nNumele clanului trebuie sa contina intre 3 si 10 caractere.", "Ok", "Back");
		}
		else if(sSlot[playerid] == 40) {
			Dialog_Show(playerid, DIALOG_CLAN, DIALOG_STYLE_INPUT, "Shop:", "Esti pe cale sa-ti creezi un clan cu 40 locuri disponibile.\nTot ce trebuie sa faci in acest moment,\
			 este sa-ti alegi numele clanului.\nNumele clanului trebuie sa contina intre 3 si 10 caractere.", "Ok", "Back");
		}
		return true;
	}
	
	switch(sSlot[playerid]) {
		case 10: {
			if(PlayerInfo[playerid][pPremiumPoints] < 2000) return SCM(playerid, -1, "Nu ai destule Premium Points.");
			else {
				PlayerInfo[playerid][pPremiumPoints] -= 2000;
				Update(playerid, pPremiumPointsx);
			}
		}	
		case 20: {
			if(PlayerInfo[playerid][pPremiumPoints] < 3000) return SCM(playerid, -1, "Nu ai destule Premium Points.");
			else {
				PlayerInfo[playerid][pPremiumPoints] -= 3000;
				Update(playerid, pPremiumPointsx);					
			}
		}	
		case 40: {
			if(PlayerInfo[playerid][pPremiumPoints] < 300) return SCM(playerid, -1, "Nu ai destule Premium Points.");
			else {
				PlayerInfo[playerid][pPremiumPoints] -= 300;
				Update(playerid, pPremiumPointsx);					
			}
		}	
	}
	
	new
		szDate[180], query[150],
		Year, Month, Day, Hour, Minute, Second;

	getdate(Year, Month, Day);
	gettime(Hour, Minute, Second);

	format(szDate, sizeof(szDate), "%02d/%02d/%d - %02d:%02d:%02d", Day, Month, Year, Hour, Minute, Second);
	mysql_format(SQL, query, sizeof(query), "INSERT INTO `clans` (`Name`, `Owner`, `OwnerSQL`, `RegisterDate`, `Color`, `Slots`) VALUES ('%e', '%e', '%d', '%e', 'FFFFFF', '%d')", inputtext, PlayerInfo[playerid][pUsername], PlayerInfo[playerid][pSQLID], szDate, sSlot[playerid]);
	new Cache: result = mysql_query(SQL, query);
	new i = cache_insert_id();
	cache_delete(result);

	ClanInfo[i][clSlots] = sSlot[playerid];
	format(ClanInfo[i][clName], 64, inputtext);
	format(ClanInfo[i][clOwner], 256, PlayerInfo[playerid][pUsername]);
	format(ClanInfo[i][clRegisterDate], 256, szDate);
	format(ClanInfo[i][clColor], 256, "FFFFFF");
	format(ClanInfo[i][clRankColor1], 64, "E3E3E2");
	format(ClanInfo[i][clRankColor2], 64, "E3E3E2");
	format(ClanInfo[i][clRankColor3], 64, "E3E3E2");
	format(ClanInfo[i][clRankColor4], 64, "E3E3E2");
	format(ClanInfo[i][clRankColor5], 64, "E3E3E2");
	format(ClanInfo[i][clRankColor6], 64, "E3E3E2");
	ClanInfo[i][cID] = i;
	ClanMembers[i] ++;
	PlayerInfo[playerid][pClan] = i;
	PlayerInfo[playerid][pClanRank] = 6;
	ClanInfo[i][clDays]	= 60;
	new szText[180];
	format(szText, sizeof(szText), "Numele ales de tine este: %s.\nCel de-al doilea pas, este de a alege tagul clanului.\nTagul clanului trebuie sa contina intre 3 si 9 caractere.", inputtext);
	Dialog_Show(playerid, DIALOG_CLAN2, DIALOG_STYLE_INPUT, "Shop:", szText, "Ok", "");

	mysql_format(SQL, query, sizeof(query), "UPDATE `users` SET `Clan`='%d', `ClanRank`='%d' WHERE `Name`='%s'", PlayerInfo[playerid][pClan], PlayerInfo[playerid][pClanRank], PlayerInfo[playerid][pUsername]);
	mysql_tquery(SQL, query, "", "");
	return 1;
}
Dialog:DIALOG_CLAN2(playerid, response, listitem, inputtext[]) {
	if(!response) {
		SCM(playerid, COLOR_WHITE, "Tagul clanului trebuie sa aiba intre 3 si 9 caractere.");
		new szText[180];
		format(szText, sizeof(szText), "Numele ales de tine este: %s.\nCel de-al doilea pas, este de a alege tagul clanului.\nTagul clanului trebuie sa contina intre 3 si 9 caractere.", ClanInfo[PlayerInfo[playerid][pClan]][clName]);
		Dialog_Show(playerid, DIALOG_CLAN2, DIALOG_STYLE_INPUT, "Shop:", szText, "Ok", "");			
		return true;
	}
	if(!ValidClanTag(inputtext)) {
		SCM(playerid, COLOR_GREY, "Tagul trebuie sa contina [ ] sau .");
		new szText[180];
		format(szText, sizeof(szText), "Numele ales de tine este: %s.\nCel de-al doilea pas, este de a alege tagul clanului.\nTagul clanului trebuie sa contina intre 3 si 9 caractere.", ClanInfo[PlayerInfo[playerid][pClan]][clName]);
		Dialog_Show(playerid, DIALOG_CLAN2, DIALOG_STYLE_INPUT, "Shop:", szText, "Ok", "");					
		return true;
	}	
	if(InvalidClanTag(inputtext)) {
		SCM(playerid, COLOR_GREY, "Caractere invalide! Tag-ul nu poate contine #, {}, @.");
		new szText[180];
		format(szText, sizeof(szText), "Numele ales de tine este: %s.\nCel de-al doilea pas, este de a alege tagul clanului.\nTagul clanului trebuie sa contina intre 3 si 9 caractere.", ClanInfo[PlayerInfo[playerid][pClan]][clName]);
		Dialog_Show(playerid, DIALOG_CLAN2, DIALOG_STYLE_INPUT, "Shop:", szText, "Ok", "");					
		return true;
	}	
	if(ExistaTagClan(inputtext)) { 
		SCM(playerid, COLOR_WARNING, "Exista deja un clan inregistrat cu acest TAG!");	
		new szText[180];
		format(szText, sizeof(szText), "Numele ales de tine este: %s.\nCel de-al doilea pas, este de a alege tagul clanului.\nTagul clanului trebuie sa contina intre 3 si 9 caractere.", inputtext);
		Dialog_Show(playerid, DIALOG_CLAN2, DIALOG_STYLE_INPUT, "Shop:", szText, "Ok", "");					
		return true;
	}
	if(strlen(inputtext) < 3 || strlen(inputtext) > 9) {
		SCM(playerid, COLOR_WHITE, "Tagul clanului trebuie sa aiba intre 3 si 9 caractere.");
		new szText[180];
		format(szText, sizeof(szText), "Numele ales de tine este: %s.\nCel de-al doilea pas, este de a alege tagul clanului.\nTagul clanului trebuie sa contina intre 3 si 9 caractere.", inputtext);
		Dialog_Show(playerid, DIALOG_CLAN2, DIALOG_STYLE_INPUT, "Shop:", szText, "Ok", "");
		return true;
	}
	new clanid = PlayerInfo[playerid][pClan], query[160];
	format(ClanInfo[clanid][clTag], 32, inputtext);
	mysql_format(SQL,query, sizeof(query), "UPDATE `clans` SET `Tag`='%s' WHERE `ID`='%d'", inputtext, clanid);
	mysql_tquery(SQL, query, "", "");

	new szName[180];
	new string[144];
	format(string, sizeof(string), "(SHOP INFO) {FFFFFF} Ti-ai cumparat un clan cu %d sloturi disponibile.", sSlot[playerid]);
	SCM(playerid, COLOR_GREEN, string);
	Update(playerid, pPremiumPointsx);
	format(string, sizeof(string), "Informatii: Numele clanului, este %s (tag: %s). [clan #%d]", ClanInfo[clanid][clName], ClanInfo[clanid][clTag], PlayerInfo[playerid][pClan]);
	SCM(playerid, COLOR_WHITE, string);

	format(ClanInfo[clanid][clRankName1], 32, "Rank 1");
	format(ClanInfo[clanid][clRankName2], 32, "Rank 2");
	format(ClanInfo[clanid][clRankName3], 32, "Rank 3");
	format(ClanInfo[clanid][clRankName4], 32, "Rank 4");
	format(ClanInfo[clanid][clRankName5], 32, "Rank 5");
	format(ClanInfo[clanid][clRankName6], 32, "Rank 6");
	format(ClanInfo[clanid][clMotd], 32, "None");

	format(szName, sizeof(szName), "%s%s", ClanInfo[clanid][clTag], PlayerInfo[playerid][pUsername]);
	SetPlayerName(playerid, szName);			
	return 1;
}
Dialog:DIALOG_CLAN_SETTINGS(playerid, response, listitem, inputtext[])
{
	if(!response) return Dialog_Show(playerid, DIALOG_CLAN_COMMAND, DIALOG_STYLE_LIST, "Clan:", "Comenzile clanului\nMembrii clanului\nSetari\nTag", "Select", "Close");
	new string[144], query[200];	
	switch(listitem)
	{
		case 0: Dialog_Show(playerid, DIALOG_CLAN_COLOR, DIALOG_STYLE_INPUT, "Setari:", "Scrie mai jos culoarea clanului. (EX: FFFFFF - ALB)\nSite recomandat: http://www.colorpicker.com/", "Select", "Back");
		case 1: Dialog_Show(playerid, DIALOG_CLAN_MOTD, DIALOG_STYLE_INPUT, "Settings", "Introdu textul pe care vrei sa-l pui la MOTD.", "Select", "Back");
		case 2: Dialog_Show(playerid, DIALOG_CLAN_RANK, DIALOG_STYLE_INPUT, "Settings", "Introdu numele pe care vrei sa-l pui la rankuri.", "Select", "Back");
		case 3: Dialog_Show(playerid, DIALOG_CLAN_COLOR_RANK, DIALOG_STYLE_INPUT, "Settings", "Scrie mai jos culoarea rankului. (EX: FFFFFF - ALB)\nSite recomandat: http://www.colorpicker.com/", "Select", "Back");
		case 4: {
			if(PlayerInfo[playerid][pPremiumPoints] < 40) return SCM(playerid, COLOR_GREY, "Ai nevoie de 40 Premium Points pentru a face aceasta schimbare!");
			Dialog_Show(playerid, DIALOG_CLANNAME, DIALOG_STYLE_INPUT, "Change clan name", "Introdu mai jos numele pe care doresti sa-l aiba clanul.\nAceasta schimbare costa 40 Premium Points, gandeste-te bine la alegere!", "Select", "Back");
		}
		case 5: {
			if(PlayerInfo[playerid][pPremiumPoints] < 40) return SCM(playerid, COLOR_GREY, "Ai nevoie de 40 Premium Points pentru a face aceasta schimbare!");
			Dialog_Show(playerid, DIALOG_CHANGETAG, DIALOG_STYLE_INPUT, "Change clan TAG", "Introdu mai jos numele pe care doresti sa-l aiba tagul clanului.\nAceasta schimbare costa 40 Premium Points, gandeste-te bine la alegere!", "Select", "Back");
		}
		case 6: {
			if(PlayerInfo[playerid][pPremiumPoints] < 50) return SCM(playerid, COLOR_GREY, "Ai nevoie de 50 Premium Points pentru a face aceasta schimbare!");
			new clanid = PlayerInfo[playerid][pClan];
			PlayerInfo[playerid][pPremiumPoints] -= 50;
			Update(playerid, pPremiumPointsx);
			ClanInfo[clanid][clDays] += 30;
			mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `Days`='%d' WHERE `ID`='%d'", ClanInfo[clanid][clDays], clanid);
			mysql_tquery(SQL, query, "", "");
			format(string, sizeof(string), "Ai marit zilele clanului cu 30 de zile. Clanul tau va expira peste %d zile.", ClanInfo[clanid][clDays]);
			SCM(playerid, COLOR_MONEY, string);
		}
	}
	return 1;
}
Dialog:DIALOG_CLANNAME(playerid, response, listitem, inputtext[]) {
	if(ExistaNumeClan(inputtext)) return SCM(playerid, COLOR_WARNING, "Exista deja un clan inregistrat cu acest nume!");	
	if(strlen(inputtext) < 3 || strlen(inputtext) > 10) return SCM(playerid, COLOR_GREY, "Numele clanului trebuie sa contina cel putin 3 caractere si maxim 10!");
	if(PlayerInfo[playerid][pPremiumPoints] < 40) return SCM(playerid, COLOR_GREY, "Ai nevoie de 40 Premium Points pentru a face aceasta schimbare!");
	PlayerInfo[playerid][pPremiumPoints] -= 40;
	Update(playerid, pPremiumPointsx);			
	new query[200];
	new clanid = PlayerInfo[playerid][pClan];
	format(ClanInfo[clanid][clName], 64, inputtext);
	mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `Name`='%s' WHERE `ID`='%d'", inputtext, clanid);
	mysql_tquery(SQL, query, "", "");
	new string[144];
	format(string, sizeof(string), "{%s}[CLAN] %s a schimbat numele clanului in %s.", ClanInfo[clanid][clColor], GetName(playerid), inputtext);
	SendClanMessage(clanid, string);
	new tagname[180];
	for(new i = 0; i < MAX_SAFEZONES; i++)
	{
		if(GraffitiInfo[i][gfOwned] == PlayerInfo[playerid][pClan]) {
			DestroyDynamicObject(GraffitiInfo[i][gfObject]);
			GraffitiInfo[i][gfObject] = CreateDynamicObject(19353, GraffitiInfo[i][gfX], GraffitiInfo[i][gfY], GraffitiInfo[i][gfZ], 0.0, 0.0, GraffitiInfo[i][gfRot]);
			format(tagname, sizeof(tagname), "{%s}%s\n%s", ClanInfo[GraffitiInfo[i][gfOwned]][clColor], ClanInfo[GraffitiInfo[i][gfOwned]][clName], ClanInfo[GraffitiInfo[i][gfOwned]][clTag]);
			SetDynamicObjectMaterialText(GraffitiInfo[i][gfObject], 0, tagname, OBJECT_MATERIAL_SIZE_512x256, "BankGothic Md BT", 60, 1, 0xfff0f0f0, 0, OBJECT_MATERIAL_TEXT_ALIGN_CENTER);
		}
	}			
	return 1;
}
Dialog:DIALOG_CHANGETAG(playerid, response, listitem, inputtext[]) {
	if(ExistaTagClan(inputtext)) return SCM(playerid, COLOR_WARNING, "Exista deja un clan inregistrat cu acest TAG!");
	if(!ValidClanTag(inputtext)) return SCM(playerid, COLOR_GREY, "Tagul trebuie sa contina [ ] sau .");
	if(InvalidClanTag(inputtext)) return SCM(playerid, COLOR_GREY, "Caractere invalide! Tag-ul nu poate contine #, {}, @.");		
	if(strlen(inputtext) < 3 || strlen(inputtext) > 10) return SCM(playerid, COLOR_GREY, "Tagul clanului trebuie sa contina cel putin 3 caractere si maxim 10!");
	if(PlayerInfo[playerid][pPremiumPoints] < 40) return SCM(playerid, COLOR_GREY, "Ai nevoie de 40 Premium Points pentru a face aceasta schimbare!");
	new clanid = PlayerInfo[playerid][pClan];
	PlayerInfo[playerid][pPremiumPoints] -= 40;
	Update(playerid, pPremiumPointsx);				
	format(ClanInfo[clanid][clTag], 64, inputtext);
	new query[200];
	mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `Tag`='%s' WHERE `ID`='%d'", inputtext, clanid);
	mysql_tquery(SQL, query, "", "");
	new string[144];
	format(string, sizeof(string), "{%s}[CLAN] %s a schimbat TAG-ul clanului in %s.", ClanInfo[clanid][clColor], GetName(playerid), inputtext);
	SendClanMessage(clanid, string);
	new tagname[180];
	for(new i = 0; i < MAX_SAFEZONES; i++)
	{
		if(GraffitiInfo[i][gfOwned] == PlayerInfo[playerid][pClan]) {
			DestroyDynamicObject(GraffitiInfo[i][gfObject]);
			GraffitiInfo[i][gfObject] = CreateDynamicObject(19353, GraffitiInfo[i][gfX], GraffitiInfo[i][gfY], GraffitiInfo[i][gfZ], 0.0, 0.0, GraffitiInfo[i][gfRot]);
			format(tagname, sizeof(tagname), "{%s}%s\n%s", ClanInfo[GraffitiInfo[i][gfOwned]][clColor], ClanInfo[GraffitiInfo[i][gfOwned]][clName], ClanInfo[GraffitiInfo[i][gfOwned]][clTag]);
			SetDynamicObjectMaterialText(GraffitiInfo[i][gfObject], 0, tagname, OBJECT_MATERIAL_SIZE_512x256, "BankGothic Md BT", 60, 1, 0xfff0f0f0, 0, OBJECT_MATERIAL_TEXT_ALIGN_CENTER);
		}
	}
	foreach(new i: Player) {
		if(PlayerInfo[i][pClan] == clanid && IsPlayerConnected(i)) {
			if(PlayerInfo[i][pTag] == 0) {
				format(string, sizeof(string), "%s%s", ClanInfo[clanid][clTag], PlayerInfo[i][pUsername]);
				SetPlayerName(i, string);
			}
			else if(PlayerInfo[i][pTag] == 1) {
				format(string, sizeof(string), "%s%s", PlayerInfo[i][pUsername], ClanInfo[clanid][clTag]);
				SetPlayerName(i, string);
			}
		}
	}		
	return 1;
}		
Dialog:DIALOG_CLAN_COLOR(playerid, response, listitem, inputtext[])
{
	new clanid = PlayerInfo[playerid][pClan];
	new query[200];	
	if(!response) return  Dialog_Show(playerid, DIALOG_CLAN_SETTINGS, DIALOG_STYLE_LIST, "Setari:", "Schimba culoarea\nSchimba MOTD-ul\nSchimba numele rankurilor\nSchimba culoarea rankurilor\nSchimba culorile rankurilor\n{FFCC00}(620 PP){FFFFFF} Schimba numele clanului\n{FFCC00}(310 PP){FFFFFF} Schimba tagul clanului\n{FFCC00}(720PP){FFFFFF} +30 Clan days", "Select", "Back");
	if(strlen(inputtext) <= 0 || strlen(inputtext) > 6) return  Dialog_Show(playerid, DIALOG_CLAN_SETTINGS, DIALOG_STYLE_LIST, "Setari:", "Schimba culoarea\nSchimba MOTD-ul\nSchimba numele rankurilor\n{FFCC00}(620 PP){FFFFFF} Schimba numele clanului\n{FFCC00}(310 PP){FFFFFF} Schimba tagul clanului\n{FFCC00}(720PP){FFFFFF} +30 Clan days", "Select", "Back");
	format(ClanInfo[clanid][clColor], 256, inputtext);
	new string[144];	
	format(string, sizeof(string), "{%s}[CLAN] %s a schimbat culoarea la clan.", ClanInfo[clanid][clColor], GetName(playerid), inputtext);
	SendClanMessage(clanid, string);
	new tagname[180];
	for(new i = 0; i < MAX_SAFEZONES; i++)
	{
		if(GraffitiInfo[i][gfOwned] == PlayerInfo[playerid][pClan]) {
			DestroyDynamicObject(GraffitiInfo[i][gfObject]);
			GraffitiInfo[i][gfObject] = CreateDynamicObject(19353, GraffitiInfo[i][gfX], GraffitiInfo[i][gfY], GraffitiInfo[i][gfZ], 0.0, 0.0, GraffitiInfo[i][gfRot]);
			format(tagname, sizeof(tagname), "{%s}%s\n%s", ClanInfo[GraffitiInfo[i][gfOwned]][clColor], ClanInfo[GraffitiInfo[i][gfOwned]][clName], ClanInfo[GraffitiInfo[i][gfOwned]][clTag]);
			SetDynamicObjectMaterialText(GraffitiInfo[i][gfObject], 0, tagname, OBJECT_MATERIAL_SIZE_512x256, "BankGothic Md BT", 60, 1, 0xfff0f0f0, 0, OBJECT_MATERIAL_TEXT_ALIGN_CENTER);
		}
	}
	if(ClanInfo[clanid][clHQ] > 0) UpdateLabel(4, ClanInfo[clanid][clHQ]);
	mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `Color`='%s' WHERE `ID`='%d'", ClanInfo[clanid][clColor], clanid);
	mysql_tquery(SQL, query, "", "");
	return 1;
}
Dialog:DIALOG_CLAN_MOTD(playerid, response, listitem, inputtext[])
{
	if(!response) return  Dialog_Show(playerid, DIALOG_CLAN_SETTINGS, DIALOG_STYLE_LIST, "Setari:", "Schimba culoarea\nSchimba MOTD-ul\nSchimba numele rankurilor\nSchimba culoarea rankurilor\n{FFCC00}(620 PP){FFFFFF} Schimba numele clanului\n{FFCC00}(310 PP){FFFFFF} Schimba tagul clanului\n{FFCC00}(720PP){FFFFFF} +30 Clan days", "Select", "Back");
	new clanid = PlayerInfo[playerid][pClan];
	new string[144];	
	format(string, sizeof(string), "{%s}[CLAN] %s a schimbat MOTD clan in %s.", ClanInfo[clanid][clColor], GetName(playerid), inputtext);
	SendClanMessage(clanid, string);
	new query[200];
	format(ClanInfo[clanid][clMotd], 32, "%s", inputtext);
	mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `Motd`='%s' WHERE `ID`='%d'", ClanInfo[clanid][clMotd], clanid);
	mysql_tquery(SQL, query, "", "");
	return 1;
}
Dialog:DIALOG_CLAN_COLOR_RANK(playerid, response, listitem, inputtext[]) {
	if(!response) return  Dialog_Show(playerid, DIALOG_CLAN_SETTINGS, DIALOG_STYLE_LIST, "Setari:", "Schimba culoarea\nSchimba MOTD-ul\nSchimba numele rankurilor\nSchimba culoarea rankurilor\n{FFCC00}(620 PP){FFFFFF} Schimba numele clanului\n{FFCC00}(310 PP){FFFFFF} Schimba tagul clanului\n{FFCC00}(720PP){FFFFFF} +30 Clan days", "Select", "Back");
	if(strlen(inputtext) <= 0 || strlen(inputtext) > 6) return  Dialog_Show(playerid, DIALOG_CLAN_SETTINGS, DIALOG_STYLE_LIST, "Setari:", "Schimba culoarea\nSchimba MOTD-ul\nSchimba numele rankurilor\n{FFCC00}(620 PP){FFFFFF} Schimba numele clanului\n{FFCC00}(310 PP){FFFFFF} Schimba tagul clanului\n{FFCC00}(720PP){FFFFFF} +30 Clan days", "Select", "Back");
	new string[144];
	format(string, sizeof(string), "%s", inputtext);
	format(szClanColor[playerid], 256, string);
	Dialog_Show(playerid, DIALOG_CLAN_COLOR_RANK1, DIALOG_STYLE_LIST, "Selecteaza rank-ul:", "Rank 1\nRank 2\nRank 3\nRank 4\nRank 5\nRank 6", "Schimba", "Back");
	return 1;
}
Dialog:DIALOG_CLAN_COLOR_RANK1(playerid, response, listitem, inputtext[]) {
	if(!response) return Dialog_Show(playerid, DIALOG_CLAN_COLOR_RANK, DIALOG_STYLE_INPUT, "Setarile clanului:", "{FFFFFF}Introdu numele pe care vrei sa-l pui la rankuri.", "Select", "Back");
	new clanid = PlayerInfo[playerid][pClan];
	new string[144], query[200];
	switch(listitem) {
		case 0: {
			format(ClanInfo[clanid][clRankColor1], 256, szClanColor[playerid]);
			mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `RankColor1`='%s' WHERE `ID`='%d'", ClanInfo[clanid][clRankColor1], clanid);
			mysql_tquery(SQL, query, "", "");
			format(string, sizeof(string), "{%s}[CLAN] %s a schimbat culoarea la rank 1 in {%s}%s.", ClanInfo[clanid][clColor], GetName(playerid), ClanInfo[clanid][clRankColor1], ClanInfo[clanid][clRankName1]);
			SendClanMessage(clanid, string);
		}
		case 1: {
			format(ClanInfo[clanid][clRankColor2], 256, szClanColor[playerid]);
			mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `RankColor2`='%s' WHERE `ID`='%d'", ClanInfo[clanid][clRankColor2], clanid);
			mysql_tquery(SQL, query, "", "");
			format(string, sizeof(string), "{%s}[CLAN] %s a schimbat culoarea la rank 2 in {%s}%s.", ClanInfo[clanid][clColor], GetName(playerid), ClanInfo[clanid][clRankColor2], ClanInfo[clanid][clRankName2]);
			SendClanMessage(clanid, string);
		}
		case 2: {
			format(ClanInfo[clanid][clRankColor3], 256, szClanColor[playerid]);
			mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `RankColor3`='%s' WHERE `ID`='%d'", ClanInfo[clanid][clRankColor3], clanid);
			mysql_tquery(SQL, query, "", "");
			format(string, sizeof(string), "{%s}[CLAN] %s a schimbat culoarea la rank 3 in {%s}%s.", ClanInfo[clanid][clColor], GetName(playerid), ClanInfo[clanid][clRankColor3], ClanInfo[clanid][clRankName3]);
			SendClanMessage(clanid, string);
		}
		case 3: {
			format(ClanInfo[clanid][clRankColor4], 256, szClanColor[playerid]);
			mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `RankColor4`='%s' WHERE `ID`='%d'", ClanInfo[clanid][clRankColor4], clanid);
			mysql_tquery(SQL, query, "", "");
		  	format(string, sizeof(string), "{%s}[CLAN] %s a schimbat culoarea la rank 4 in {%s}%s.", ClanInfo[clanid][clColor], GetName(playerid), ClanInfo[clanid][clRankColor4], ClanInfo[clanid][clRankName4]);
			SendClanMessage(clanid, string);
		}
		case 4: {
			format(ClanInfo[clanid][clRankColor5], 256, szClanColor[playerid]);
			mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `RankColor5`='%s' WHERE `ID`='%d'", ClanInfo[clanid][clRankColor5], clanid);
			mysql_tquery(SQL, query, "", "");
			format(string, sizeof(string), "{%s}[CLAN] %s a schimbat culoarea la rank 5 in {%s}%s.", ClanInfo[clanid][clColor], GetName(playerid), ClanInfo[clanid][clRankColor5], ClanInfo[clanid][clRankName5]);
			SendClanMessage(clanid, string);
		}
		case 5: {
			format(ClanInfo[clanid][clRankColor6], 256, szClanColor[playerid]);
			mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `RankColor6`='%s' WHERE `ID`='%d'", ClanInfo[clanid][clRankColor6], clanid);
			mysql_tquery(SQL, query, "", "");
			format(string, sizeof(string), "{%s}[CLAN] %s a schimbat culoarea la rank 6 in {%s}%s.", ClanInfo[clanid][clColor], GetName(playerid), ClanInfo[clanid][clRankColor6], ClanInfo[clanid][clRankName6]);
			SendClanMessage(clanid, string);
		}
	}			
	return 1;
}
Dialog:DIALOG_CLAN_RANK(playerid, response, listitem, inputtext[]) {
	if(!response) return  Dialog_Show(playerid, DIALOG_CLAN_SETTINGS, DIALOG_STYLE_LIST, "Setari:", "Schimba culoarea\nSchimba MOTD-ul\nSchimba numele rankurilor\nSchimba culoarea rankurilor\n{FFCC00}(620 PP){FFFFFF} Schimba numele clanului\n{FFCC00}(310 PP){FFFFFF} Schimba tagul clanului\n{FFCC00}(720PP){FFFFFF} +30 Clan days", "Select", "Back");
	new string[144];
	format(string, sizeof(string), "%s", inputtext);
	format(szRank[playerid], 256, string);
	Dialog_Show(playerid, DIALOG_CLAN_RANK2, DIALOG_STYLE_LIST, "Selecteaza rank-ul:", "Rank 1\nRank 2\nRank 3\nRank 4\nRank 5\nRank 6", "Schimba", "Back");
	return 1;
}
Dialog:DIALOG_CLAN_RANK2(playerid, response, listitem, inputtext[])
{
	if(!response) return Dialog_Show(playerid, DIALOG_CLAN_RANK, DIALOG_STYLE_INPUT, "Setarile clanului:", "{FFFFFF}Introdu numele pe care vrei sa-l pui la rankuri.", "Select", "Back");
	new
		ClanRank,
		clanid = PlayerInfo[playerid][pClan];
	new string[144], query[200];
	switch(listitem) {
		case 0: {
			ClanRank = 1;
			format(ClanInfo[clanid][clRankName1], 32, "%s", szRank[playerid]);
			mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `RankName1`='%s' WHERE `ID`='%d'", ClanInfo[clanid][clRankName1], clanid);
			mysql_tquery(SQL, query, "", "");
		}
		case 1: {
			ClanRank = 2;
			format(ClanInfo[clanid][clRankName2], 32, "%s", szRank[playerid]);
			mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `RankName2`='%s' WHERE `ID`='%d'", ClanInfo[clanid][clRankName2], clanid);
			mysql_tquery(SQL, query, "", "");
		}
		case 2: {
			ClanRank = 3;
			format(ClanInfo[clanid][clRankName3], 32, "%s", szRank[playerid]);
			mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `RankName3`='%s' WHERE `ID`='%d'", ClanInfo[clanid][clRankName3], clanid);
			mysql_tquery(SQL, query, "", "");
		}
		case 3: {
			ClanRank = 4;
			format(ClanInfo[clanid][clRankName4], 32, "%s", szRank[playerid]);
			mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `RankName4`='%s' WHERE `ID`='%d'", ClanInfo[clanid][clRankName4], clanid);
			mysql_tquery(SQL, query, "", "");
		}
		case 4: {
			ClanRank = 5;
			format(ClanInfo[clanid][clRankName5], 32, "%s", szRank[playerid]);
			mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `RankName5`='%s' WHERE `ID`='%d'", ClanInfo[clanid][clRankName5], clanid);
			mysql_tquery(SQL, query, "", "");
		}
		case 5: {
			ClanRank = 6;
			format(ClanInfo[clanid][clRankName6], 32, "%s", szRank[playerid]);
			mysql_format(SQL, query, sizeof(query), "UPDATE `clans` SET `RankName6`='%s' WHERE `ID`='%d'", ClanInfo[clanid][clRankName6], clanid);
			mysql_tquery(SQL, query, "", "");
		}
	}
	format(string, sizeof(string), "{%s}[CLAN] %s a schimbat name rank %d in %s.", ClanInfo[clanid][clColor], GetName(playerid), ClanRank, szRank[playerid]);
	SendClanMessage(clanid, string);
	return 1;
}
Dialog:DIALOG_SHOPMENU(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	ShopType[playerid] = listitem;
	ShowShop(playerid);
	return 1;
}
Dialog:DIALOG_SHOP(playerid, response, listitem, inputtext[]) {
	new szDialog[1100];
	if(!response) { InShop[playerid] = 0; Dialog_Show(playerid, DIALOG_SHOPMENU, DIALOG_STYLE_TABLIST_HEADERS, "Shop Menu", "Shop Type\tItem\n{FFEA3B}[+] Shop Info{FFFFFF}\nNormal Shop\t17 items\nVip Cars\t9 items\nExtra Peacks\t3 items\nIce Cream Shop\t8 items", "Select", "Cancel"); return true; }
	switch(ShopType[playerid]) {
		case 1: {
			switch(listitem) {
				case 0: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un cont premium?\nPentru asta, ai nevoie de 150 puncte premium.\n\n\
					Avantaje:\n-Interes 0.2 la payday\n- 2 RP-uri o data la 5 ore\n- Poti avea 5 masini personale in total\n\
					- numarul (/carplate) o sa fie de culoare portocalie.\n- acces la comanda /pcolor.\n- Poti pune pana la 100 obiecte in casa\nChat (/pc).");
					SetPVarInt(playerid, "PremiumPoints", 150);
				}
				case 1: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi %s?\n{FFFFFF}Pentru asta, ai nevoie de %d Premium Points.", UpgradeVip(PlayerInfo[playerid][pVip]+1), PriceVIP(PlayerInfo[playerid][pVip]+1));
					SetPVarInt(playerid, "PremiumPoints", PriceVIP(PlayerInfo[playerid][pVip]+1));
				}
				case 2: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un clan ?\nPentru asta, ai nevoie de 300 Premium Points.\n\n\
					Avantaje:\n- Clan disponibil 60 zile (se poate prelungi)\n- Poti invita 40 playeri");
					SetPVarInt(playerid, "PremiumPoints", 300);
				}
				case 3: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti schimbi numele?\nPentru asta, ai nevoie de 50 Premium Points. ");
					SetPVarInt(playerid, "PremiumPoints", 50);
				}				
				case 4: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi o culoare hidden?\nPentru asta, ai nevoie de 40 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 40);
				}				
				case 5: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un slot de vehicule?\nCosta 40 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 40);
				}
				case 6: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti stergi un warn?\nPentru asta, ai nevoie de 30 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 30);
				}	
				case 7: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti stergi kilometrii la unul dintre vehiculele tale personale?\nPentru asta, ai nevoie de 35 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 35);
				}				
				case 8: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un MP3?\nPentru asta, ai nevoie de 20 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 20);
				}
				case 9: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un iPhone?\nPentru asta, ai nevoie de 20 Premium Points.\n\n\
					Avantaje:\n- Numar format din 4 cifre\n- Texte modificate la /call respectiv /sms\n- Acces la comanda /reply (/re).");
					SetPVarInt(playerid, "PremiumPoints", 20);
				}
				case 10: {
					new bouse = PlayerInfo[playerid][pHouse];
					if(bouse != 999 && strcmp(GetName(playerid), HouseInfo[PlayerInfo[playerid][pHouse]][hOwner], true) == 0) {
						if(HouseInfo[PlayerInfo[playerid][pHouse]][hRadio] == 1) return SCM(playerid, -1, "Aceasta afacere are deja un radio!");
						format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi radio in casa?\nCosta 15 Premium Points.");
						SetPVarInt(playerid, "PremiumPoints", 15);
					}
					else return SCM(playerid, COLOR_GREY, "Nu ai o casa!");
				}
				case 11: {
					if(PlayerInfo[playerid][pBizz] == 255) return SCM(playerid, -1, "Nu ai o afacere!");
					if(BizzInfo[PlayerInfo[playerid][pBizz]][bRadio] == 1) return SCM(playerid, -1, "Aceasta afacere are deja un radio!");				
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi radio in afacere?\nCosta 15 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 15);
				}	
				case 12: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un Boost job?\nCosta 20 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 20);
				}
				case 13: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un Rare Vocuher?\nCosta 55 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 55);
				}
				case 14: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un Epic Vocuher?\nCosta 110 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 110);
				}
				case 15: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un Legendary Vocuher?\nCosta 525 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 525);
				}
				case 16: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un Clear FP?\nCosta 99 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 99);
				}
			}
			Dialog_Show(playerid, DIALOG_SHOP1, DIALOG_STYLE_MSGBOX, "Shop: Normal", szDialog, "Ok", "Back");		
		}
		case 2: {
			switch(listitem) {
				case 0: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un Infernus Premium?\nAcesta costa 5600 Premium points, Acesta prinde viteaza de 300 km/h");
					SetPVarInt(playerid, "PremiumPoints", 5600);
				}
				case 1: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un Hotering A?\nAcesta costa 5400 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 5400);
				}
				case 2: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un Hotering B?\nAcesta costa 5400 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 5400);
				}
				case 3: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un Hotering C?\nAcesta costa 5400 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 5400);
				}				
				case 4: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un Streach?\nPentru asta, ai nevoie de 5450 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 5450);
				}				
				case 5: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un Sparrow?\nCosta 5400 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 5400);
				}
				case 6: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi un Maverick?\nPentru asta, ai nevoie de 5500 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 5500);
				}	
				case 7: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi Shamal?\nPentru asta, ai nevoie de 5800 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 5800);
				}		
				case 8: {
					format(szDialog, sizeof(szDialog), "Esti sigur ca vrei sa-ti achizitionezi Monster?\nPentru asta, ai nevoie de 6200 Premium Points.");
					SetPVarInt(playerid, "PremiumPoints", 6200);
				}			
			}
			Dialog_Show(playerid, DIALOG_SHOP1, DIALOG_STYLE_MSGBOX, "Shop: VIP Cars", szDialog, "Ok", "Back");			
		}
		case 3: {
			switch(listitem) {
				case 0: {	
					format(szDialog, sizeof(szDialog), "INFO: Daca deti unul dintre aceste produse nu le vei primi!\n\nCe primesc dupa ce cumpar acest pachet? (1000 Premium Points)\n> Vip User\n> Premium Account\n> iPhone\n> +$20,000,000");
					SetPVarInt(playerid, "PremiumPoints", 1000);							
				}
			   	case 1: {	
					format(szDialog, sizeof(szDialog), "INFO: Daca deti unul dintre aceste produse nu le vei primi!\n\nCe primesc dupa ce cumpar acest pachet? (850 Premium Points)\n1x Legendary Voucher\n2x Epic Voucher\n3x Rare Voucher");
					SetPVarInt(playerid, "PremiumPoints", 850);							
				}
				case 2: {	
					format(szDialog, sizeof(szDialog), "INFO: Daca deti unul dintre aceste produse nu le vei primi!\n\nCe primesc dupa ce cumpar acest pachet? (1000 Premium Points)\n> Hotring Racer A/B/C\n4x Hidden Color\n1x Slot");
					SetPVarInt(playerid, "PremiumPoints", 500);							
				}
			}	
			Dialog_Show(playerid, DIALOG_SHOP1, DIALOG_STYLE_MSGBOX, "Shop: Extra Packs", szDialog, "Ok", "Back");			
		}
	}
	SelectedItem[playerid] = listitem;
	return 1;
}
Dialog:DIALOG_SHOP1(playerid, response, listitem, inputtext[]) {
	if(!response) { InShop[playerid] = 0; ShowShop(playerid); return true; }
	new string[144], query[200];
	switch(ShopType[playerid]) {
		case 1: {
			new gold = GetPVarInt(playerid, "PremiumPoints");
			if(PlayerInfo[playerid][pPremiumPoints] < gold) return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Shop", "Nu ai suficiente Premium Points pentru a achizitiona acest produs.", "Ok", "");
			switch(SelectedItem[playerid]) {
				case 0: {
					if(PlayerInfo[playerid][pPremiumAccount] == 1) return SCM(playerid, -1, "Contul tau deja este Premium.");
			  		SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Contul tau, este acum, unul de tip Premium.");
					PlayerInfo[playerid][pPremiumAccount] = 1;
					Update(playerid, pPremiumAccountx);
					PlayerInfo[playerid][pPremiumPoints]-=100;
					format(string, sizeof(string), "%s si-a achizitionat cont premium pentru 100 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
				}
				case 1: {
					if(PlayerInfo[playerid][pVip] == 2) return SCM(playerid, -1, "Ai upgradat la maximum");
					PlayerInfo[playerid][pVip] ++;
					PlayerInfo[playerid][pBackpack] = 2;
					SetPlayerAttachedObject( playerid, 8, 3026, 1, -0.172813, -0.112569, -0.022387, 0.000000, 0.000000, 3.390071, 1.000000, 1.000000, 1.130721);
					Update(playerid, pVipx);
					Update(playerid, pBackpackx);
					switch(PlayerInfo[playerid][pVip]) {
						case 1: {
							SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Contul tau este acum unul de tip VIP.");
							Update3DTextLabelText(vipLabel[playerid], 0x7DE41BFF, "VIP USER");
						}
						case 2: {
							SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai upgradat contul la un VIP Plus.");
							Update3DTextLabelText(vipLabel[playerid], 0xF28036FF, "VIP USER");
						}
					}
					PlayerInfo[playerid][pPremiumPoints] -= PriceVIP(PlayerInfo[playerid][pVip]);
					Attach3DTextLabelToPlayer(vipLabel[playerid], playerid, 0.0, 0.0, 0.4);
					format(string, sizeof(string), "%s si-a achizitionat un cont %s pentru %d Premium Points.", GetName(playerid), UpgradeVip(PlayerInfo[playerid][pVip]), PriceVIP(PlayerInfo[playerid][pVip]));
					InsertLog(playerid, string, LOG_SHOP);						
				}	
				case 2: {
					if(PlayerInfo[playerid][pClanRank] == 6) return SCM(playerid, COLOR_WHITE, "Ai deja un clan.");
					if(PlayerInfo[playerid][pClan] != 0) return SCM(playerid, COLOR_WHITE, "Faci parte dintr-un clan deja.");
					sSlot[playerid] = 40;
					Dialog_Show(playerid, DIALOG_CLAN, DIALOG_STYLE_INPUT, "Shop:", "Esti pe cale sa-ti creezi un clan cu 40 locuri disponibile.\nTot ce trebuie sa faci in acest moment,\
					 este sa-ti alegi numele clanului.\nNumele clanului trebuie sa contina intre 3 si 10 caractere.", "Ok", "");
					format(string, sizeof(string), "%s si-a achizitionat un clan mare pentru 300 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);				   
				}
				case 3: Dialog_Show(playerid, DIALOG_CHANGENAME, DIALOG_STYLE_INPUT, "Shop:", "Esti pe cale sa-ti schimbi numele.\nTot ce trebuie sa faci in acest moment, este sa-ti alegi un nume care nu este luat de altcineva.\nLimita este de 3 si 24 caractere.", "Ok", "Back");
				case 4: {
					PlayerInfo[playerid][pHidden] ++;
					UpdateVar(playerid, "Hidden", PlayerInfo[playerid][pHidden]);
					PlayerInfo[playerid][pPremiumPoints] -= 30;
					SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai cumparat o culoare hidden pentru 30 premium points.");
					format(string, sizeof(string), "%s si-a achizitionat o culoare hidden pentru %d Premium Points.", GetName(playerid), GetPVarInt(playerid, "PremiumPoints"));
					InsertLog(playerid, string, LOG_SHOP);	
				}
				case 5: {
					if(GetSlots(playerid) >= TotalSlots(playerid)) return SCM(playerid, -1, "Ai numarul maxim de sloturi!");
					PlayerInfo[playerid][pSlot] ++;
					PlayerInfo[playerid][pPremiumPoints] -= 20; 
					Update(playerid, pPremiumPointsx); 
					UpdateSlots(playerid);
					SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Ti-ai cumparat un slot pentru vehicule in schimbul a 20 Premium Points.");
					format(string, sizeof(string), "%s si-a achizitionat un slot (vehicul) pentru 20 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);						
				}
				case 6: {
					if(PlayerInfo[playerid][pWarns] == 0) return SCM(playerid, COLOR_GREY, "Nu ai warn-uri.");
					PlayerInfo[playerid][pWarns] -= 1;
					UpdateVar(playerid, "Warnings", PlayerInfo[playerid][pWarns]);
					PlayerInfo[playerid][pPremiumPoints] -= 30;
					format(string, sizeof(string), "%s si-a sters un Warn pentru %d Premium Points.", GetName(playerid), GetPVarInt(playerid, "PremiumPoints"));
					InsertLog(playerid, string, LOG_SHOP);					
				}		
				case 7: {
					if(!IsPlayerInAnyVehicle(playerid)) return SCM(playerid, COLOR_GREY, "Nu esti intr-un vehicul!");
					new idd;
					if(Iter_Count(MyVehicle[playerid]) == 0) return SCM(playerid, COLOR_WHITE, "Acest vehicul nu iti apartine!");
					idd = PersonalCar(playerid);
					if(idd == -1) return SCM(playerid, COLOR_WHITE, "Acest vehicul nu iti apartine!");
					pCar[playerid] = idd;
					Dialog_Show(playerid, DIALOG_RESETKM, DIALOG_STYLE_MSGBOX, "Shop:", "Esti sigur ca vrei sa resetezi kilometrii acestui vehicul?", "Da", "Back");
				}					
				case 8: {
					if(PlayerInfo[playerid][pMP3] == 1) return SCM(playerid,COLOR_WHITE, "Ai deja un radio.");
					PlayerInfo[playerid][pMP3] = 1;
					SCM(playerid,COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai cumparat un MP3 din magazin. Foloseste /mp3 pentru a asculta muzica.");
					Update(playerid, pRadiox);
					PlayerInfo[playerid][pPremiumPoints] -= GetPVarInt(playerid, "PremiumPoints");
					format(string, sizeof(string), "%s si-a achizitionat un MP3 pentru %d Premium Points.", GetName(playerid), GetPVarInt(playerid, "PremiumPoints"));
					InsertLog(playerid, string, LOG_SHOP);					
				}				
				case 9: {
					Dialog_Show(playerid, DIALOG_IPHONE, DIALOG_STYLE_INPUT, "Shop:", "Esti pe cale sa-ti achizitionezi un iPhone.\nTot ce trebuie sa faci in acest moment, este sa-ti alegi un numar format din 4 cifre.", "Ok", "Back");
				}
				case 10: {
					if(PlayerInfo[playerid][pPremiumPoints] < 15) return SCM(playerid, -1, "Nu ai suficiente Premium Points!");
					PlayerInfo[playerid][pPremiumPoints] -= 15;
					Update(playerid, pPremiumPointsx);					
					HouseInfo[PlayerInfo[playerid][pHouse]][hRadio] = 0;
					format(query, sizeof(query), "UPDATE `houses` SET `Radio`='1' WHERE `ID`='%d'", PlayerInfo[playerid][pHouse]);
					SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Acum poti pune radio in casa ta! Foloseste comanda /hradio pentru a pune.");
					format(string, sizeof(string), "%s si-a achizitionat un radio (casa) pentru 15 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);				
				}
				case 11: {
					if(PlayerInfo[playerid][pPremiumPoints] < 15) return SCM(playerid, -1, "Nu ai suficiente Premium Points!");
					PlayerInfo[playerid][pPremiumPoints] -= 15;
					Update(playerid, pPremiumPointsx);						
					if(BizzInfo[PlayerInfo[playerid][pBizz]][bStatic] == 1) return SCM(playerid, -1, "Nu poti pune radio in aceasta afacere.");
					BizzInfo[PlayerInfo[playerid][pBizz]][bRadio] = 0;
					format(query, sizeof(query), "UPDATE `bizz` SET `Radio`='1' WHERE `ID`='%d'", PlayerInfo[playerid][pBizz]);
					SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Acum poti pune radio in afacerea ta! Foloseste comanda /bradio pentru a pune.");
					format(string, sizeof(string), "%s si-a achizitionat un radio (bizz) pentru 15 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);						
				}	
				case 12: {
					if(PlayerInfo[playerid][pPremiumPoints] < 20) return SCM(playerid, -1, "Nu ai suficiente Premium Points");
					PlayerInfo[playerid][pPremiumPoints] -= 20;
					PlayerInfo[playerid][pJobBoost] += 5;
					SetPVarInt(playerid, "PremiumPoints", PlayerInfo[playerid][pPremiumPoints]);
					SCMEx(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari %s ti-ai cumparat 5x Boost job pentru suma de 20 Premium Points!",GetName(playerid));
					format(string, sizeof(string), "%s si-a achizitionat 5x Boost job pentru 20 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					Update(playerid, pJobBoostx);
				}
				case 13: {
					if(PlayerInfo[playerid][pPremiumPoints] < 55) return SCM(playerid, -1, "Nu ai suficiente Premium Points");
					PlayerInfo[playerid][pPremiumPoints] -= 55;
					PlayerInfo[playerid][pVoucher][0] ++;
					SetPVarInt(playerid, "PremiumPoints", PlayerInfo[playerid][pPremiumPoints]);
					SCMEx(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari %s ti-ai cumparat un Rare Voucher pentru suma de 55 Premium Points!",GetName(playerid));
					format(string, sizeof(string), "%s si-a achizitionat un Rare Voucher pentru 55 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					save_vouchers(playerid);
				}
				case 14: {
					if(PlayerInfo[playerid][pPremiumPoints] < 110) return SCM(playerid, -1, "Nu ai suficiente Premium Points");
					PlayerInfo[playerid][pPremiumPoints] -= 110;
					PlayerInfo[playerid][pVoucher][1] ++;
					SetPVarInt(playerid, "PremiumPoints", PlayerInfo[playerid][pPremiumPoints]);
					SCMEx(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari %s ti-ai cumparat un Epic Voucher pentru suma de 110 Premium Points!",GetName(playerid));
					format(string, sizeof(string), "%s si-a achizitionat un Epic Voucher pentru 110 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					save_vouchers(playerid);
				}
				case 15: {
					if(PlayerInfo[playerid][pPremiumPoints] < 525) return SCM(playerid, -1, "Nu ai suficiente Premium Points");
					PlayerInfo[playerid][pPremiumPoints] -= 525;
					PlayerInfo[playerid][pVoucher][2] ++;
					SetPVarInt(playerid, "PremiumPoints", PlayerInfo[playerid][pPremiumPoints]);
					SCMEx(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari %s ti-ai cumparat un Legendary Voucher pentru suma de 525 Premium Points!",GetName(playerid));
					format(string, sizeof(string), "%s si-a achizitionat un Legendary Voucher pentru 525 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					save_vouchers(playerid);
				}
				case 16: {
					if(PlayerInfo[playerid][pPremiumPoints] < 99) return SCM(playerid, -1, "Nu ai suficiente Premium Points");
					PlayerInfo[playerid][pPremiumPoints] -= 99;
					PlayerInfo[playerid][pFpunish] = 0;	
					UpdateVar(playerid, "FPunish", 0);
					SCMEx(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari %s ti-ai cumparat un Clear FP pentru suma de 99 Premium Points!",GetName(playerid));
					format(string, sizeof(string), "%s si-a achizitionat un Clear FP pentru 99 Premium Points.", GetName(playerid));
				}
			}
			Update(playerid, pPremiumPointsx);
		}
		case 2: {
			new gold = GetPVarInt(playerid, "PremiumPoints");
			if(PlayerInfo[playerid][pPremiumPoints] < gold) return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Shop", "Nu ai suficiente Premium Points pentru a achizitiona acest produs.", "Ok", "");
			switch(SelectedItem[playerid]) {
				case 0: {
					if(ServerStock[0] == 0) return SCM(playerid, COLOR_LGREEN, "Nu sunt stockuri disponibile.");
					if(Iter_Count(MyVehicle[playerid]) >= GetSlots(playerid)) return SCM(playerid, -1, "Nu ai un slot liber!");
			  		SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai cumparat un Infernus Premium, pentru 5600 Premium Points.");
					PlayerInfo[playerid][pPremiumPoints] -= 5600;
					GiveVehicle(playerid, 411, 0, 0, 1, 0, 0);
					gString[0] = EOS;
					format(gString, sizeof(gString), "Shop: %s si-a cumparat un Infernus Premium.", GetName(playerid));
					SendAdminMessage(COLOR_YELLOW, gString, 1); 
					format(string, sizeof(string), "%s si-a achizitionat un Infernus Premium pentru 5600 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					SendAdminMessage(COLOR_YELLOW, string, 1);
					ServerStock[0] --;
				}
		   		case 1: {
					if(Iter_Count(MyVehicle[playerid]) >= GetSlots(playerid)) return SCM(playerid, -1, "Nu ai un slot liber!");
			  		SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai cumparat un Hotring Racer A, pentru 5400 Premium Points.");
					PlayerInfo[playerid][pPremiumPoints] -= 5400;
					GiveVehicle(playerid, 494, 0, 0, 0, 0, 0);
					gString[0] = EOS;
					format(gString, sizeof(gString), "Shop: %s si-a cumparat un Hotring Racer A.", GetName(playerid));
					SendAdminMessage(COLOR_YELLOW, gString, 1);
					format(string, sizeof(string), "%s si-a achizitionat un Hotring Racer A pentru 5400 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					SendAdminMessage(COLOR_YELLOW, string, 1);
				}
				case 2: {
					if(Iter_Count(MyVehicle[playerid]) >= GetSlots(playerid)) return SCM(playerid, -1, "Nu ai un slot liber!");
			  		SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai cumparat un Hotring Racer B, pentru 5400 Premium Points.");
					PlayerInfo[playerid][pPremiumPoints] -= 5400;
					GiveVehicle(playerid, 502, 0, 0, 0, 0, 0);
					gString[0] = EOS;
					format(gString, sizeof(gString), "Shop: %s si-a cumparat un Hotring Racer B.", GetName(playerid));
					SendAdminMessage(COLOR_YELLOW, gString, 1);
					format(string, sizeof(string), "%s si-a achizitionat un Hotring Racer B pentru 5400 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					SendAdminMessage(COLOR_YELLOW, string, 1);
				}
				case 3: {
					if(Iter_Count(MyVehicle[playerid]) >= GetSlots(playerid)) return SCM(playerid, -1, "Nu ai un slot liber!");
			  		SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai cumparat un Hotring Racer C, pentru 5400 Premium Points.");
					PlayerInfo[playerid][pPremiumPoints] -= 5400;
					GiveVehicle(playerid, 503, 0, 0, 0, 0, 0);
					gString[0] = EOS;
					format(gString, sizeof(gString), "Shop: %s si-a cumparat un Hotring Racer C.", GetName(playerid));
					SendAdminMessage(COLOR_YELLOW, gString, 1);
					format(string, sizeof(string), "%s si-a achizitionat un Hotring Racer C pentru 5400 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					SendAdminMessage(COLOR_YELLOW, string, 1);
				}
				case 4: {
					if(Iter_Count(MyVehicle[playerid]) >= GetSlots(playerid)) return SCM(playerid, -1, "Nu ai un slot liber!");
			  		SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai cumparat un Streatch, pentru 5450 Premium Points.");
					PlayerInfo[playerid][pPremiumPoints] -= 5450;
					GiveVehicle(playerid, 409, 0, 0, 0, 0, 0);
					gString[0] = EOS;
					format(gString, sizeof(gString), "Shop: %s si-a cumparat un Streatch.", GetName(playerid));
					SendAdminMessage(COLOR_YELLOW, gString, 1);
					format(string, sizeof(string), "%s si-a achizitionat un Streatch pentru 5450 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					SendAdminMessage(COLOR_YELLOW, string, 1);
				}
				case 5: {
					if(Iter_Count(MyVehicle[playerid]) >= GetSlots(playerid)) return SCM(playerid, -1, "Nu ai un slot liber!");
			  		SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai cumparat un Spawrrow, pentru 5400 Premium Points.");
					PlayerInfo[playerid][pPremiumPoints] -= 5400;
					GiveVehicle(playerid, 447, 0, 0, 0, 0, 0);
					gString[0] = EOS;
					format(gString, sizeof(gString), "Shop: %s si-a cumparat un Spawrrow.", GetName(playerid));
					SendAdminMessage(COLOR_YELLOW, gString, 1);
					format(string, sizeof(string), "%s si-a achizitionat un Spawrrow pentru 5400 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					SendAdminMessage(COLOR_YELLOW, string, 1);
				}
				case 6: {
					if(Iter_Count(MyVehicle[playerid]) >= GetSlots(playerid)) return SCM(playerid, -1, "Nu ai un slot liber!");
			  		SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai cumparat un Maverick, pentru 5500 Premium Points.");
					PlayerInfo[playerid][pPremiumPoints] -= 5500;
					GiveVehicle(playerid, 487, 0, 0, 0, 0,0);
					gString[0] = EOS;
					format(gString, sizeof(gString), "Shop: %s si-a cumparat un Maverick.", GetName(playerid));
					SendAdminMessage(COLOR_YELLOW, gString, 1);
					format(string, sizeof(string), "%s si-a achizitionat un Maverick pentru 5500 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					SendAdminMessage(COLOR_YELLOW, string, 1);
				}
			   	case 7: {
					if(Iter_Count(MyVehicle[playerid]) >= GetSlots(playerid)) return SCM(playerid, -1, "Nu ai un slot liber!");
			  		SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai cumparat un Shamal, pentru 5800 Premium Points.");
					PlayerInfo[playerid][pPremiumPoints] -= 5800;
					GiveVehicle(playerid, 519, 0, 0, 0, 0, 0);
					gString[0] = EOS;
					format(gString, sizeof(gString), "Shop: %s si-a cumparat un Shamal.", GetName(playerid));
					SendAdminMessage(COLOR_YELLOW, gString, 1);
					format(string, sizeof(string), "%s si-a achizitionat un Shamal pentru 5800 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					SendAdminMessage(COLOR_YELLOW, string, 1);
				}
				case 8: {
					if(Iter_Count(MyVehicle[playerid]) >= GetSlots(playerid)) return SCM(playerid, -1, "Nu ai un slot liber!");
			  		SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai cumparat un Monster pentru 6200 Premium Points.");
					PlayerInfo[playerid][pPremiumPoints] -= 6200;
					GiveVehicle(playerid, 444, 0, 0, 0, 0, 0);
					gString[0] = EOS;
					format(gString, sizeof(gString), "Shop: %s si-a cumparat un Monster.", GetName(playerid));
					SendAdminMessage(COLOR_YELLOW, gString, 1);
					format(string, sizeof(string), "%s si-a achizitionat un Monster pentru 6200 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					SendAdminMessage(COLOR_YELLOW, string, 1);
				}
			}
			Update(playerid, pPremiumPointsx);
		}
		case 3: {
			new gold = GetPVarInt(playerid, "PremiumPoints");
			if(PlayerInfo[playerid][pPremiumPoints] < gold) return Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "Shop", "Nu ai suficiente Premium Points pentru a achizitiona acest produs.", "Ok", "");
			switch(SelectedItem[playerid]) {
				case 0: {
			  		SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai cumparat un Starter Pack si ai primit: Legendary user, Premium Account, iPhone, $20,000,000.");
			  		GivePlayerCash(playerid, 1, 20000000);
			  		Update(playerid, pCashx);
					PlayerInfo[playerid][pPremiumPoints] -= 1000;
					Dialog_Show(playerid, DIALOG_IPHONE, DIALOG_STYLE_INPUT, "Shop:", "Tot ce trebuie sa faci in acest moment, este sa-ti alegi un numar format din 4 cifre.", "Ok", "Back");
				  	PlayerInfo[playerid][pPremiumAccount] = 1;
					PlayerInfo[playerid][pVip] = 1;
					PlayerInfo[playerid][pBackpack] = 2;
					Update(playerid, pVipx);
					Update(playerid, pBackpackx);
					Update(playerid, pPremiumAccountx);
					SetPlayerAttachedObject(playerid, 8, 3026, 1, -0.172813, -0.112569, -0.022387, 0.000000, 0.000000, 3.390071, 1.000000, 1.000000, 1.130721);
					format(string, sizeof(string), "%s si-a achizitionat un Starter Pack pentru suma de 1000 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
				}
		   		case 1: {
			  		SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai cumparat un Voucher Pack si ai primit: 1x Legendary Voucher, 2x Epic Vhoucher, 3x Rare Voucher");
					PlayerInfo[playerid][pPremiumPoints] -= 850;
					PlayerInfo[playerid][pVoucher][0] += 3;
					PlayerInfo[playerid][pVoucher][1] += 2;
					PlayerInfo[playerid][pVoucher][2] ++;
					format(string, sizeof(string), "%s si-a achizitionat un Voucher Pack pentru suma de 850 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					save_vouchers(playerid);
				}
				case 2: {
			  		SCM(playerid, COLOR_GREEN, "(SHOP INFO){FFFFFF} Felicitari! Ti-ai cumparat un Vehicle Pack si ai primit: Hotring Race A/B/C, 4x Hidden, 1x Slot Vehicle.");
					PlayerInfo[playerid][pPremiumPoints] -= 500;
					PlayerInfo[playerid][pHidden] += 4;

					if(GetSlots(playerid) != TotalSlots(playerid)) 
						PlayerInfo[playerid][pSlot] ++, UpdateSlots(playerid);
					
					format(string, sizeof(string), "%s si-a achizitionat un Vehicle Pack pentru 500 Premium Points.", GetName(playerid));
					InsertLog(playerid, string, LOG_SHOP);
					if(Iter_Count(MyVehicle[playerid]) != GetSlots(playerid)) Dialog_Show(playerid, DIALOG_SHOPMENU1, DIALOG_STYLE_LIST, "Vehicle Pack", "Hotring Race A\nHotring Race B\nHotring Race C", "Select", "");
				}				
			}
			Update(playerid, pPremiumPointsx);
		}
	}
	return 1;
}
Dialog:DIALOG_SHOPMENU1(playerid, response, listitem, inputtext[]) {
	if(!response) return Dialog_Show(playerid, DIALOG_SHOPMENU1, DIALOG_STYLE_LIST, "Vehicle Pack", "Hotring Race A\nHotring Race B\nHotring Race C", "Select", "");
	switch(listitem) {
		case 0: GiveVehicle(playerid, 494, 0, 0, 0, 0, 0);
		case 1: GiveVehicle(playerid, 502, 0, 0, 0, 0, 0);
		case 2: GiveVehicle(playerid, 503, 0, 0, 0, 0, 0);
	}
	return 1;
}
Dialog:DIALOG_VOUCHERS(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	switch(listitem) {
		case 0: {
			if(PlayerInfo[playerid][pVoucher][0] <= 0) return SCM(playerid, COLOR_RED, "Eroare: {FFFFFF}Nu ai un voucher de acest tip!");
			PlayerInfo[playerid][pVoucher][0] --;
			PlayerInfo[playerid][pPremiumPoints] += 50;
			SCM(playerid, COLOR_LIGHTGREEN, "Vouchers: {FFFFFF}Ai deschis un Rare Voucher si ai primit 50 Premium Points.");
			save_vouchers(playerid);
			Update(playerid, pPremiumPointsx);
		}
		case 1: {
			if(PlayerInfo[playerid][pVoucher][1] <= 0) return SCM(playerid, COLOR_RED, "Eroare: {FFFFFF}Nu ai un voucher de acest tip!");
			PlayerInfo[playerid][pVoucher][1] --;
			PlayerInfo[playerid][pPremiumPoints] += 100;
			SCM(playerid, COLOR_LIGHTGREEN, "Vouchers: {FFFFFF}Ai deschis un Epic Voucher si ai primit 100 Premium Points.");
			save_vouchers(playerid);
			Update(playerid, pPremiumPointsx);
		}
		case 2: {
			if(PlayerInfo[playerid][pVoucher][2] <= 0) return SCM(playerid, COLOR_RED, "Eroare: {FFFFFF}Nu ai un voucher de acest tip!");
			PlayerInfo[playerid][pVoucher][2] --;
			PlayerInfo[playerid][pPremiumPoints] += 500;
			SCM(playerid, COLOR_LIGHTGREEN, "Vouchers: {FFFFFF}Ai deschis un Legendary Voucher si ai primit 500 Premium Points.");
			save_vouchers(playerid);
			Update(playerid, pPremiumPointsx);
		}
	}
	return 1;
}
Dialog:DIALOG_BUYCRATES(playerid, response, listitem, inputtext[]) {
	if(!response) 
		return true;

	format(gString, sizeof(gString), "Payment\tPrice\nRPoints\t{E26AF2}%d\nPremium Points\t{EEEE3F}%d", CratePrice[AlegeCrates[playerid]][0], CratePrice[AlegeCrates[playerid]][1]);
	Dialog_Show(playerid, DIALOG_BUYCRATES1, DIALOG_STYLE_TABLIST_HEADERS, "Buy Crate", gString, "Da", "nu");
	return 1;
}
Dialog:DIALOG_BUYCRATES1(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new x = AlegeCrates[playerid];
	switch(listitem) {
		case 0: {
			if(PlayerInfo[playerid][pHpoints] < CratePrice[x][0]) return SCM(playerid, -1, "Nu ai suficiente RPoints");
			PlayerInfo[playerid][pHpoints] -= CratePrice[x][0];
			format(gString, sizeof(gString), "(Crate Systems){FFFFFF} Ti-ai cumparat o cutie (%s) pentru {E26AF2}%d RPoints{FFFFFF}! Total cutii rare: %d [%d total - general]", CrateName[x], CratePrice[x][0], PlayerInfo[playerid][pCrates][x]+1, totalcrates(playerid)+1);
			Update(playerid, pHpointsx);
		}
		case 1: {
			if(PlayerInfo[playerid][pPremiumPoints] < CratePrice[x][1]) return SCM(playerid, -1, "Nu ai suficiente puncte premium.");
			PlayerInfo[playerid][pPremiumPoints] -= CratePrice[x][1];
			format(gString, sizeof(gString), "(Crate Systems){FFFFFF} Ti-ai cumparat o cutie (%s) pentru {EEEE3F}%d PP{FFFFFF}! Total cutii rare: %d [%d total - general]", CrateName[x], CratePrice[x][1], PlayerInfo[playerid][pCrates][x]+1, totalcrates(playerid)+1);		
			Update(playerid, pPremiumPointsx);
		}
	}		
	SCM(playerid, 0xD8AADCFF, gString);
	if(x == 4) {
		ServerStock[1] --;
		format(gString, sizeof(gString), "{597F9C}(Stock: %d)\n{20F5F1}Vehicle Crate{FFFFFF}\nUse {20F5F1}/buycrate{FFFFFF} to buy crate\n{FFFFFF}Crate Price: {20F5F1}7000 RPoints {FFFFFF} / {20F5F1}700 Premium Points", ServerStock[1]);
		UpdateDynamic3DTextLabelText(LabelCrates, -1, gString);	
	}	
	SCM(playerid, -1, "Pentru a deschide o cuite, foloseste /opencrate.");
	PlayerInfo[playerid][pCrates][x] ++;
	save_crates(playerid);
	return 1;
}
Dialog:DIALOG_IPHONE(playerid, response, listitem, inputtext[]) {
	if(!response) return ShowShop(playerid);
	if(ShopType[playerid] != 3) {
		if(PlayerInfo[playerid][pPremiumPoints] < 20) return SCM(playerid, COLOR_WHITE, "Ai nevoie de 20 Premium Points pentru a lua acest produs.");
	}
	if(strlen(inputtext) != 4 || strval(inputtext) < 1000) {
			SCM(playerid, -1, "Numarul trebuie sa fie format din 4 cifre si sa inceapa de la 1000.");
			Dialog_Show(playerid, DIALOG_IPHONE, DIALOG_STYLE_INPUT, "Shop:", "Esti pe cale sa-ti {5F9CC9}achizitionezi un iPhone{FFFFFF}.\nTot ce trebuie sa faci in acest moment, este sa-ti alegi un numar format din 4 cifre.", "Ok", "Back");
			return true;
	}
	new string[144];	
	new szQuery[256];
	format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `PhoneNr`='%d' LIMIT 1", strval(inputtext));
	new Cache: result = mysql_query(SQL, szQuery);
	new test = cache_num_rows();
	cache_delete(result);
	if(test != 0) {
 		SCM(playerid, -1, "Acest numar este deja luat.");
		Dialog_Show(playerid, DIALOG_IPHONE, DIALOG_STYLE_INPUT, "Shop:", "Esti pe cale sa-ti achizitionezi un iPhone.\nTot ce trebuie sa faci in acest moment, este sa-ti alegi un numar format din 4 cifre.", "Ok", "Back");
		return true;
	}
		
	PlayerInfo[playerid][pPhone] = strval(inputtext);
	Update(playerid, pPnumberx);
	if(ShopType[playerid] != 3) {
		format(string, sizeof(string), "{2584CA}(SHOP){FFFFFF} Ti-ai cumparat un iPhone pentru 20 Premium Points. Numarul achizitionat: %s", inputtext);
		SCM(playerid, COLOR_GOLD, string);
		PlayerInfo[playerid][pPremiumPoints] -= 20;
		format(string, sizeof(string), "%s si-a achizitionat un iPhone (%d) pentru 20 Premium Points.", GetName(playerid), strval(inputtext));
		InsertLog(playerid, string, LOG_SHOP);	
	}	
	else format(string, sizeof(string), "Numarul tau ales este: %s", inputtext), SCM(playerid, COLOR_WHITE, string);
	Update(playerid, pPremiumPointsx);	
	return 1;
}		
Dialog:DIALOG_CHANGENAME(playerid, response, listitem, inputtext[]) {
	if(!response) return ShowShop(playerid);
	if(PlayerInfo[playerid][pPremiumPoints] < 50) return SCM(playerid, -1, "Nu ai suficiente Premium Points!");
	if(strlen(inputtext) < 2 || strlen(inputtext) > 24) {
		SCM(playerid, COLOR_GREY, "Numele trebuie sa aiba intre 3 si 24 caractere.");
		Dialog_Show(playerid, DIALOG_CHANGENAME, DIALOG_STYLE_INPUT, "Shop:", "Esti pe cale sa-ti creezi schimbi numele}.\nTot ce trebuie sa faci in acest moment, este sa-ti alegi un nume care nu este luat de altcineva.\nLimita este de 3 si 24 caractere.", "Ok", "Back");
	}	
	new szQuery[256];
	mysql_format(SQL, szQuery, sizeof(szQuery), "UPDATE `friends` SET `friendName`='%s' WHERE `friendName`='%s'", inputtext, GetName(playerid));
	mysql_tquery(SQL, szQuery, "", "");
	format(szQuery, sizeof(szQuery), "SELECT * FROM `users` WHERE `name`='%s' LIMIT 1", inputtext);
	new Cache: result = mysql_query(SQL, szQuery);
	new test = cache_num_rows();
	cache_delete(result);
	if(test != 0) {
		Dialog_Show(playerid, DIALOG_CHANGENAME, DIALOG_STYLE_INPUT, "Shop:", "Esti pe cale sa-ti creezi schimbi numele.\nTot ce trebuie sa faci in acest moment, este sa-ti alegi un nume care nu este luat de altcineva.\nLimita este de 3 si 24 caractere.", "Ok", "Back");
		SCM(playerid, COLOR_WHITE, "Acest nume este deja luat. Alege-ti altul!");
		return true;
	}			
	PlayerInfo[playerid][pPremiumPoints] -= 50;
	Update(playerid, pPremiumPointsx);
	new string[144];
	format(string, sizeof(string), "{2584CA}(SHOP){FFFFFF} Felicitari! Ti-ai schimbat numele in %s.", inputtext);
	SCM(playerid, COLOR_GOLD, string);
	
	if(PlayerInfo[playerid][pHouse] != 999 && strcmp(GetName(playerid), HouseInfo[PlayerInfo[playerid][pHouse]][hOwner], true) == 0) {
		new houseid = PlayerInfo[playerid][pHouse];
		strmid(HouseInfo[houseid][hOwner], inputtext, 0, strlen(inputtext), 255);
		if(HouseInfo[houseid][hGarage] != 0) {
			strmid(GarageInfo[HouseInfo[houseid][hGarage]][gOwner], inputtext, 0, strlen(inputtext), 255);
			UpdateLabel(3, HouseInfo[houseid][hGarage]);
			mysql_format(SQL, string, sizeof(string), "UPDATE `garages` SET `Owner`='%s' WHERE `ID`='%d'",inputtext, HouseInfo[houseid][hGarage]);
			mysql_tquery(SQL,string, "", "");
		}
		mysql_format(SQL, string, sizeof(string), "UPDATE `houses` SET `Owner`='%s' WHERE `ID`='%d'",inputtext,houseid);
		mysql_tquery(SQL,string, "", "");
		UpdateLabel(1,PlayerInfo[playerid][pHouse]);
	}
	if(PlayerInfo[playerid][pBizz] != 255) {
		new bizid2 = PlayerInfo[playerid][pBizz];
		strmid(BizzInfo[bizid2][bOwner], inputtext, 0, strlen(inputtext), 255);
		mysql_format(SQL, string, sizeof(string), "UPDATE `bizz` SET `Owner`='%s' WHERE `ID`='%d'",inputtext,bizid2);
		mysql_tquery(SQL,string, "", "");
		UpdateLabel(2,PlayerInfo[playerid][pBizz]);
	}
	new clanid = PlayerInfo[playerid][pClan];
	new lastname[32];
	format(lastname, sizeof(lastname), "%s", PlayerInfo[playerid][pUsername]);
	GetPlayerName(playerid, PlayerInfo[playerid][pUsername], MAX_PLAYER_NAME);
	if(clanid != 0) SCM(playerid, COLOR_GOLD, "Deoarece faci parte dintr-un clan, tagul ti-a fost dat inapoi fara sa se intample vreo problema.");
	if(PlayerInfo[playerid][pClan] != 0 && PlayerInfo[playerid][pTag] == 0) {
		format(string, sizeof(string), "%s%s", ClanInfo[clanid][clTag], PlayerInfo[playerid][pUsername]);
		SetPlayerName(playerid, string);
	}
	else if(PlayerInfo[playerid][pClan] != 0 && PlayerInfo[playerid][pTag] == 1) {
		format(string, sizeof(string), "%s%s", PlayerInfo[playerid][pUsername], ClanInfo[clanid][clTag]);
		SetPlayerName(playerid, string);
	}	
	new query[200];
	foreach(new id: MyVehicle[playerid]) {
		mysql_format(SQL, query, sizeof(query), "UPDATE `cars` SET `Owner`='%s' WHERE `ID`='%d'", inputtext, CarInfo[id][cID]);
		mysql_tquery(SQL, query, "", "");
		format(CarInfo[id][cOwner], 32, inputtext);	
	}
	
	
	format(string, sizeof(string), "%s si-a schimbat numele in %s pentru 25 Premium Points.", lastname, inputtext);
	InsertLog(playerid, string, LOG_SHOP);
				
	mysql_format(SQL, query, sizeof(query), "UPDATE `users` SET `Name`='%s' WHERE `ID`='%d'", inputtext, PlayerInfo[playerid][pSQLID]);
	mysql_tquery(SQL, query, "", "");
	Kick(playerid);
	return 1;
}
Dialog:DIALOG_SHOW(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new szDialog[512];
	for(new i = 0; i < 12; i++) format(szDialog, sizeof(szDialog), "%s%s\n", szDialog, HudName[i]);
	switch(listitem) {
		case 0: {
			if(PlayerInfo[playerid][pShowJob] == 0) {
				SCM(playerid, 0xFFADADFF, "(HUD) Job Info was updated. New status is {0AC917}Enabled{FFADAD}.");
				PlayerInfo[playerid][pShowJob] = 1;
			}
			else {
				SCM(playerid, 0xFFADADFF, "(HUD) Job Info was updated. New status is {FF0000}Disable{FFADAD}.");
				PlayerInfo[playerid][pShowJob] = 0;
				if(JobWorking[playerid] == 1) PlayerTextDrawHide(playerid, JobTD);
			}
		}
		case 1: {
			PlayerInfo[playerid][pShowCeas] = PlayerInfo[playerid][pShowCeas] ? false : true;
			SCM(playerid, 0xFFADADFF, PlayerInfo[playerid][pShowCeas] ? "(HUD) Time played was updated. New status is {0AC917}Enabled{FFADAD}." : "(HUD) Time played was updated. New status is {FF0000}Disable{FFADAD}.");
			Update(playerid, pShowCeasx);
			if(PlayerInfo[playerid][pShowCeas] == 1) PlayerTextDrawShow(playerid, CeasPTD), TextDrawShowForPlayer(playerid, CeasTD);
			else PlayerTextDrawHide(playerid, CeasPTD), TextDrawHideForPlayer(playerid, CeasTD);
			return true;
		}
		case 2: {
			PlayerInfo[playerid][pShowFP] = PlayerInfo[playerid][pShowFP] ? (false) : (true);
			SCM(playerid, 0xFFADADFF, PlayerInfo[playerid][pShowFP] ? "(HUD) FPS and Ping was updated. New status is {0AC917}Enabled{FFADAD}." : "(HUD) FPS and Ping was updated. New status is {FF0000}Disable{FFADAD}.");
			if(PlayerInfo[playerid][pShowFP] == 1) PlayerTextDrawShow(playerid, ShowFPS);
			else PlayerTextDrawHide(playerid, ShowFPS);
		}
		case 3: SelectedHud[playerid] = 0, Dialog_Show(playerid, DIALOG_HUDSELECT,DIALOG_STYLE_LIST,"{FFDD99}HUD {FFFFFF}Menu:",szDialog,"Select","Cancel");
		case 4: SelectedHud[playerid] = 1, Dialog_Show(playerid, DIALOG_HUDSELECT,DIALOG_STYLE_LIST,"{FFDD99}HUD {FFFFFF}Menu:",szDialog,"Select","Cancel"); 			
		case 5: SelectedHud[playerid] = 2, Dialog_Show(playerid, DIALOG_HUDSELECT,DIALOG_STYLE_LIST,"{FFDD99}HUD {FFFFFF}Menu:",szDialog,"Select","Cancel"); 
		case 6: {
			if(PlayerInfo[playerid][pShowDMG] == 0) {
				PlayerInfo[playerid][pShowDMG] = 1;
				SCM(playerid, 0xFFADADFF, "(HUD) Damage Informer was updated. New status is {0AC917}Enabled{FFADAD}.");
			}
			else {
				PlayerInfo[playerid][pShowDMG] = 0;
				SCM(playerid, 0xFFADADFF, "(HUD) Damage Informer was updated. New status is {FF0000}Disable{FFADAD}.");
			}
			UpdateVar(playerid, "ShowDMG", PlayerInfo[playerid][pShowDMG]);
		}				
		case 7: {
			if(PlayerInfo[playerid][pShowBanca] == 0) {
				SCM(playerid, 0xFFADADFF, "(HUD) Bank Money Textdraw was updated. New status is {0AC917}Enabled{FFADAD}.");
				PlayerInfo[playerid][pShowBanca] = 1;
				new string[200];
				format(string, sizeof(string), "$%s", GetBankMoney(playerid));
				PlayerTextDrawSetString(playerid, BancaTD, string);
				PlayerTextDrawShow(playerid, BancaTD);					
			}
			else {
				SCM(playerid, 0xFFADADFF, "(HUD) Bank Money Textdraw was updated. New status is {FF0000}Disable{FFADAD}.");
				PlayerInfo[playerid][pShowBanca] = 0;
				PlayerTextDrawHide(playerid, BancaTD);
			}
			UpdateVar(playerid, "ShowBanca", PlayerInfo[playerid][pShowBanca]);
		}
		case 8: {
			if(PlayerInfo[playerid][pShowProgress][0] == 0) {
				PlayerInfo[playerid][pShowProgress][0] = 1;
				SCM(playerid, 0xFFADADFF, "(HUD) Level Progress was updated. New status is {0AC917}Enabled{FFADAD}.");	
				HudProgress[playerid][0] = CreatePlayerProgressBar(playerid, 270.00, 430.00, 105.50, 18.20, 0x00000090, 100.0);	
				UpdateProgress(playerid);		
			}
			else {
				PlayerInfo[playerid][pShowProgress][0] = 0;
				SCM(playerid, 0xFFADADFF, "(HUD) Level Progress was updated. New status is {FF0000}Disable{FFADAD}.");
				DestroyPlayerProgressBar(playerid, HudProgress[playerid][0]);
			}
			UpdateVar(playerid, "ShowProgress1", PlayerInfo[playerid][pShowProgress][0]);
		}
		case 9: {
			if(PlayerInfo[playerid][pHudGen] == 0) {
				PlayerInfo[playerid][pHudGen] = 1;
				SCM(playerid, 0xFFADADFF, "(HUD) Speedomter was updated. New status is {0AC917}Enabled{FFADAD}.");
			}
			else {
				PlayerInfo[playerid][pHudGen] = 0;
				SCM(playerid, 0xFFADADFF, "(HUD) Speedomter was updated. New status is {FF0000}Disable{FFADAD}.");
				for(new i = 0; i < 3; i++) PlayerTextDrawHide(playerid, SpeedometerPTD[i]);
			}
			Update(playerid, pHudGenx);
		}	
		case 10: {
			if(PlayerInfo[playerid][pShowProgress][2] == 0) {
				SCM(playerid, 0xFFADADFF, "(HUD) Job Goal was updated. New status is {0AC917}Enabled{FFADAD}.");
				PlayerInfo[playerid][pShowProgress][2] = 1;			
			   	for(new i = 0; i < 2; i++) TextDrawShowForPlayer(playerid, GoalTD[i]);
			}
			else {
				SCM(playerid, 0xFFADADFF, "(HUD) Job Goal was updated. New status is {FF0000}Disable{FFADAD}.");
				PlayerInfo[playerid][pShowProgress][2] = 0;
				for(new i = 0; i < 2; i++) TextDrawHideForPlayer(playerid, GoalTD[i]);
			}
			UpdateVar(playerid, "ShowProgress3", PlayerInfo[playerid][pShowProgress][2]);
		}
	}
	Update(playerid, pShowJobx);
	Update(playerid, pShowFPx);
	Update(playerid, pShowLogox);
	return 1;
}
Dialog:DIALOG_HUDSELECT(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	switch(SelectedHud[playerid]) {
		case 0: PlayerInfo[playerid][pShowHP] = listitem;
		case 1: PlayerInfo[playerid][pShowAP] = listitem;	
		case 2: {
			togalert[playerid] = listitem;
			UpdateVar(playerid, "TogAlert", listitem);
		}
		case 3: {
			PlayerInfo[playerid][pShowProgress][0] = listitem;
			UpdateVar(playerid, "ShowProgress1", listitem);
			if(listitem == 0) DestroyProgress(playerid, 0);
			else UpdateProgress(playerid);
		}			
	}
	SCM(playerid, 0xFFADADFF, "Optiuni HUD actualizate!");
	Update(playerid, pShowHPx);
	Update(playerid, pShowAPx);		
	return 1;		
}			
Dialog:DIALOG_EDITJOB(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(PlayerInfo[playerid][pAdmin] < 6) return true;
	switch(listitem) {
		case 0: Dialog_Show(playerid, DIALOG_EDITJOB1, DIALOG_STYLE_INPUT, "Job Name:", "Scrie putin mai jos valoarea pe care vrei sa o aiba aceasta variabila.", "Ok", "Close");
		case 1: Dialog_Show(playerid, DIALOG_EDITJOB1, DIALOG_STYLE_INPUT, "Level:", "Scrie putin mai jos valoarea pe care vrei sa o aiba aceasta variabila.", "Ok", "Close");
		case 2: Dialog_Show(playerid, DIALOG_EDITJOB1, DIALOG_STYLE_INPUT, "Tip:", "Scrie putin mai jos valoarea pe care vrei sa o aiba aceasta variabila.", "Ok", "Close");
		case 3: Dialog_Show(playerid, DIALOG_EDITJOB1, DIALOG_STYLE_INPUT, "Skill 1 Veh:", "Scrie putin mai jos valoarea pe care vrei sa o aiba aceasta variabila.", "Ok", "Close");
		case 4: Dialog_Show(playerid, DIALOG_EDITJOB1, DIALOG_STYLE_INPUT, "Skill 2 Veh:", "Scrie putin mai jos valoarea pe care vrei sa o aiba aceasta variabila.", "Ok", "Close");
		case 5: Dialog_Show(playerid, DIALOG_EDITJOB1, DIALOG_STYLE_INPUT, "Skill 3 Veh:", "Scrie putin mai jos valoarea pe care vrei sa o aiba aceasta variabila.", "Ok", "Close");
		case 6: Dialog_Show(playerid, DIALOG_EDITJOB1, DIALOG_STYLE_INPUT, "Skill 4 Veh:", "Scrie putin mai jos valoarea pe care vrei sa o aiba aceasta variabila.", "Ok", "Close");
		case 7: Dialog_Show(playerid, DIALOG_EDITJOB1, DIALOG_STYLE_INPUT, "Skill 5 Veh:", "Scrie putin mai jos valoarea pe care vrei sa o aiba aceasta variabila.", "Ok", "Close");
		case 8: Dialog_Show(playerid, DIALOG_EDITJOB1, DIALOG_STYLE_INPUT, "Skill 6 Veh:", "Scrie putin mai jos valoarea pe care vrei sa o aiba aceasta variabila.", "Ok", "Close");
	}
	EditJob[playerid] = listitem;
	return 1;
}
Dialog:DIALOG_EDITJOB1(playerid, response, listitem, inputtext[]) {
	new
		amount = strval(inputtext),
		job = EditJobID[playerid];

	if(!response) {
		new szDialog[1024];
		if(PlayerInfo[playerid][pAdmin] < 6) 
			return SCM(playerid, COLOR_LGREEN, AdminOnly);
		
		format(szDialog, sizeof(szDialog),
			"Job Name\t\t%s\nLevel\t\t%d\nLegal\t\t%d\n\
			Skilll 1 Veh\t\t%d\nSkill 2 Veh\t\t%d\nSkill 3 Veh\t\t%d\nSkill 4 Veh\t\t%d\nSkill 5 Veh\t\t%d\nSkill 6 Veh\t\t%d",
		JobInfo[job][jName], JobInfo[job][jLevel], JobInfo[job][jLegal], JobInfo[job][jSkill1Veh], JobInfo[job][jSkill2Veh], JobInfo[job][jSkill3Veh], JobInfo[job][jSkill4Veh],
		JobInfo[job][jSkill5Veh], JobInfo[job][jSkill6Veh]);

		gString[0] = EOS;
		format(gString, sizeof(gString), "Edit job %d", job);
		Dialog_Show(playerid, DIALOG_EDITJOB, DIALOG_STYLE_TABLIST, gString, szDialog, "Select", "Cancel");
		EditJobID[playerid] = job;
		return true;
	}
	if(PlayerInfo[playerid][pAdmin] < 6) 
		return true;

	gString[0] = EOS;
	gQuery[0] = EOS;
	switch(EditJob[playerid]) {
		case 0: {
			format(JobInfo[job][jName], 64, inputtext);

			format(gString, sizeof(gString), "Ai modificat 'Job Name' la jobul cu ID-ul %d, in: %s.", job, inputtext);
			SCM(playerid, COLOR_WHITE, gString);

			DestroyDynamic3DTextLabel(JobInfo[job][jLabel]);
			DestroyDynamicPickup(JobInfo[job][jPickup]);

			format(gString, sizeof(gString), "{ffffff}ID: {FF0000}%d\n{ffffff}Job: {FF0000}%s{FFFFFF}\nPress {FF0000}Y{FFFFFF} to get the job\nPress {FF0000}N{FFFFFF} to quit job", job, JobInfo[job][jName]);
			JobInfo[job][jLabel] = CreateDynamic3DTextLabel(gString, 0xFFFFFF00, JobInfo[job][jPosX], JobInfo[job][jPosY], JobInfo[job][jPosZ], 25, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, -1, -1, 100.0);
			JobInfo[job][jPickup] = CreateDynamicPickup(1239, 23, JobInfo[job][jPosX], JobInfo[job][jPosY], JobInfo[job][jPosZ], 0, -1, -1, 100.0);
			
			mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `jobs` SET `Name`='%s' WHERE `ID`='%d'", inputtext, job);
			mysql_tquery(SQL, gQuery, "", "");
		}
		case 1: {
			JobInfo[job][jLevel] = amount;
			DestroyDynamic3DTextLabel(JobInfo[job][jLabel]);
			DestroyDynamicPickup(JobInfo[job][jPickup]);

			format(gString, sizeof(gString), "{ffffff}ID: {FF0000}%d\n{ffffff}Job: {FF0000}%s{FFFFFF}\nPress {FF0000}Y{FFFFFF} to get the job\nPress {FF0000}N{FFFFFF} to quit job", job, JobInfo[job][jName]);
			JobInfo[job][jLabel] = CreateDynamic3DTextLabel(gString, 0xFFFFFF00, JobInfo[job][jPosX], JobInfo[job][jPosY], JobInfo[job][jPosZ], 25, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, -1, -1, 100.0);
			JobInfo[job][jPickup] = CreateDynamicPickup(1239, 23, JobInfo[job][jPosX], JobInfo[job][jPosY], JobInfo[job][jPosZ], 0, -1, -1, 100.0);
			
			format(gString, sizeof(gString), "Ai modificat 'Level' la jobul cu ID-ul %d, in: %d.", job, amount);
			SCM(playerid, COLOR_WHITE, gString);

			mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `jobs` SET `NeedLevel`='%d' WHERE `ID`='%d'", amount, job);
			mysql_tquery(SQL, gQuery, "", "");
		}
		case 2: {
			JobInfo[job][jLegal] = amount;

			format(gString, sizeof(gString), "Ai modificat 'Legal' la jobul cu ID-ul %d, in: %d.", job, amount);
			SCM(playerid, COLOR_WHITE, gString);

			mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `jobs` SET `Legal`='%d' WHERE `ID`='%d'", amount, job);
			mysql_tquery(SQL, gQuery, "", "");
		}
		case 3: {
			JobInfo[job][jSkill1Veh] = amount;

			format(gString, sizeof(gString), "Ai modificat 'Skill1Veh' la jobul cu ID-ul %d, in: %d.", job, amount);
			SCM(playerid, COLOR_WHITE, gString);

			mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `jobs` SET `Skill1Veh`='%d' WHERE `ID`='%d'", amount, job);
			mysql_tquery(SQL, gQuery, "", "");
		}
		case 4: {
			JobInfo[job][jSkill2Veh] = amount;

			format(gString, sizeof(gString), "Ai modificat 'Skill2Veh' la jobul cu ID-ul %d, in: %d.", job, amount);
			SCM(playerid, COLOR_WHITE, gString);

			mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `jobs` SET `Skill2Veh`='%d' WHERE `ID`='%d'", amount, job);
			mysql_tquery(SQL, gQuery, "", "");
		}
		case 5: {
			JobInfo[job][jSkill3Veh] = amount;
			format(gString, sizeof(gString), "Ai modificat 'Skill3Veh' la jobul cu ID-ul %d, in: %d.", job, amount);
			SCM(playerid, COLOR_WHITE, gString);

			mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `jobs` SET `Skill3Veh`='%d' WHERE `ID`='%d'", amount, job);
			mysql_tquery(SQL, gQuery, "", "");
		}
		case 6: {
			JobInfo[job][jSkill4Veh] = amount;
			format(gString, sizeof(gString), "Ai modificat 'Skill4Veh' la jobul cu ID-ul %d, in: %d.", job, amount);
			SCM(playerid, COLOR_WHITE, gString);

			mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `jobs` SET `Skill4Veh`='%d' WHERE `ID`='%d'", amount, job);
			mysql_tquery(SQL, gQuery, "", "");
		}
		case 7: {
			JobInfo[job][jSkill5Veh] = amount;
			format(gString, sizeof(gString), "Ai modificat 'Skill5Veh' la jobul cu ID-ul %d, in: %d.", job, amount);
			SCM(playerid, COLOR_WHITE, gString);

			mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `jobs` SET `Skill5Veh`='%d' WHERE `ID`='%d'", amount, job);
			mysql_tquery(SQL, gQuery, "", "");
		}
		case 8: {
			JobInfo[job][jSkill6Veh] = amount;
			format(gString, sizeof(gString), "Ai modificat 'Skill6Veh' la jobul cu ID-ul %d, in: %d.", job, amount);
			SCM(playerid, COLOR_WHITE, gString);

			mysql_format(SQL, gQuery, sizeof(gQuery), "UPDATE `jobs` SET `Skill6Veh`='%d' WHERE `ID`='%d'", amount, job);
			mysql_tquery(SQL, gQuery, "", "");
		}
	}
	return 1;
}
Dialog:DIALOG_JOBS(playerid, response, listitem, inputtext[]) {
	if(!response) 
		return true;
	
	if(CP[playerid] != 0) 
		return Dialog_Show(playerid, DIALOG_CHECKPOINT, DIALOG_STYLE_MSGBOX, "Checkpoint", "Ai deja un checkpoint activ.\nDoresti sa-l anulezi? Daca da, apasa pe 'Ok'.", "Ok", "Exit");
	
	new j = listitem + 1;

	gString[0] = EOS;
	format(gString, sizeof(gString), "Ti-a fost pus un checkpoint catre job-ul %s. Distanta: %0.2fm", JobInfo[j][jName], GetPlayerDistanceFromPoint(playerid, JobInfo[j][jPosX], JobInfo[j][jPosY], JobInfo[j][jPosZ]));
	SCM(playerid, COLOR_YELLOW, gString);

	SetPlayerCheckpointEx(playerid, JobInfo[j][jPosX], JobInfo[j][jPosY], JobInfo[j][jPosZ], 1);
	CP[playerid] = 53;
	return 1;
}
Dialog:DIALOG_TRUCKER(playerid, response, listitem, inputtext[]) {
	if(!response) {
		JobWorking[playerid] = 0;
		PlayerTextDrawHide(playerid, JobTD);
		if(JobVehicle[playerid] != 0) { DestroyVehicle(JobVehicle[playerid]); JobVehicle[playerid] = 0; }
		if(Trailer[playerid] != 0) { DestroyVehicle(Trailer[playerid]); Trailer[playerid] = 0;  }
		PlayerTextDrawHide(playerid, InfosTD);
		DisablePlayerCheckpointEx(playerid);
		CP[playerid] = 0;
		JobVehicle[playerid] = 0;
		SCM(playerid, COLOR_GREY, "Job esuat! Ai parasit vehiculul.");
		return true;
	}
	if(JobWorking[playerid] == 0) return true;
	
	if(listitem >= 3) {	
		new string[300];
		strcat(string, "Category\tType\tSkill\n");
		strcat(string, "Clothes\t{35DE1F}Legal\t1+\n");
		strcat(string, "Food\t{35DE1F}Legal\t1+\n");
		strcat(string, "Furniture\t{35DE1F}Legal\t1+\n");
		strcat(string, "Explosible\t{35DE1F}Legal\t2+\n");
		strcat(string, "Guns\t{F21313}Illegal\t3+\n");
		strcat(string, "Stolen cars\t{F21313}Illegal\t3+\n");
		if(listitem == 3 && GetPlayerSkill(playerid) < 2) return SCM(playerid, COLOR_LGREEN, "Eroare: Pentru a face acel transport ai nevoie de skill 2+!"), Dialog_Show(playerid, DIALOG_TRUCKER, DIALOG_STYLE_TABLIST_HEADERS, "Choose destination", string, "Select", "Cancel");
		if(listitem == 4 && GetPlayerSkill(playerid) < 3) return SCM(playerid, COLOR_LGREEN, "Eroare: Pentru a face acel transport ai nevoie de skill 3+!"), Dialog_Show(playerid, DIALOG_TRUCKER, DIALOG_STYLE_TABLIST_HEADERS, "Choose destination", string, "Select", "Cancel");
		if(listitem == 5 && GetPlayerSkill(playerid) < 3) return SCM(playerid, COLOR_LGREEN, "Eroare: Pentru a face acel transport ai nevoie de skill 3+!"), Dialog_Show(playerid, DIALOG_TRUCKER, DIALOG_STYLE_TABLIST_HEADERS, "Choose destination", string, "Select", "Cancel");
	}
		
	CP[playerid] = 52;

	new Float:pX,Float:pY,Float:pZ,Float:vA;
	GetPlayerPos(playerid,pX,pY,pZ);
	GetVehicleZAngle(GetPlayerVehicleID(playerid),vA);
	switch(listitem) {
		case 0: Trailer[playerid] = CreateVehicleEx(435, pX+6, pY+6, pZ+6, vA, -1, -1, -1, 0);
		case 2: Trailer[playerid] = CreateVehicleEx(591, pX+6, pY+6, pZ+6, vA, -1, -1, -1, 0);
		case 3: {
			Trailer[playerid] = CreateVehicleEx(584, pX+6, pY+6, pZ+6, vA, -1, -1, -1, 0);
			SetVehicleHealth(Trailer[playerid], 500);
		}
		default: Trailer[playerid] = CreateVehicleEx(450, pX+6, pY+6, pZ+6, vA, -1, -1, -1, 0);
	}

	
	// cp pos

	
	new rand = random(4);
	SetPVarInt(playerid, "Category", listitem);
	if(GetPVarInt(playerid, "Route") == rand) rand = 0;
	else SetPVarInt(playerid, "Route", rand);
	switch(listitem) {
		case 0: {
			// clohes
			switch(rand) {
				case 0: SetPlayerCheckpointEx(playerid, 456.1087,-1501.3405,31.0429, 5);
				case 1: SetPlayerCheckpointEx(playerid, 2246.9468,-1658.5454,15.2856, 5);
				case 2: SetPlayerCheckpointEx(playerid, 2062.6563,2251.4827,10.1718, 5); 
				case 3: SetPlayerCheckpointEx(playerid, 1387.6379,1038.9611,10.8203, 5);
				case 4: SetPlayerCheckpointEx(playerid, 2572.0181,1891.6405,10.8280, 5);
			}
		}
		case 1: {
			// Food
			switch(rand) {
				case 0: SetPlayerCheckpointEx(playerid, 2627.4238,1670.3981,10.9104, 5);
				case 1: SetPlayerCheckpointEx(playerid, 1426.4849,1029.8025,10.1870, 5);
				case 2: SetPlayerCheckpointEx(playerid, 2546.5735,1968.1238,10.8203, 5); 
				case 3: SetPlayerCheckpointEx(playerid, 786.4791,-1617.8722,13.3828, 5);
				case 4: SetPlayerCheckpointEx(playerid, 1193.0499,-882.8945,43.0620, 5);
			}
		}
		case 2: {
			// Furniture
			switch(rand) {
				case 0: SetPlayerCheckpointEx(playerid, 1529.4952,17.8272,24.1406, 5);
				case 1: SetPlayerCheckpointEx(playerid, 167.7361,-22.3520,1.6817, 5);
				case 2: SetPlayerCheckpointEx(playerid, 1200.6061,144.7607,20.6738, 5); 
				case 3: SetPlayerCheckpointEx(playerid, 2415.6680,89.8848,26.5133, 5);
				case 4: SetPlayerCheckpointEx(playerid, 1665.9354,731.1528,10.8203, 5);
			}
		}	
		case 3: {
			// Explosib
			switch(rand) {
				case 0: SetPlayerCheckpointEx(playerid, 646.4252,1249.9377,11.6735, 5);
				case 1: SetPlayerCheckpointEx(playerid, 2634.4856,1072.2454,10.8203, 5);
				case 2: SetPlayerCheckpointEx(playerid, 1922.2708,-1791.1569,13.3828, 5); 
				case 3: SetPlayerCheckpointEx(playerid, -81.8883,-1193.4836,1.8445, 5);
				case 4: SetPlayerCheckpointEx(playerid, 2182.7463,2489.1960,10.8203, 5);
			}
		}	
		case 4: {
			// Guns
			switch(rand) {
				case 0: SetPlayerCheckpointEx(playerid, 2183.0911,915.0748,10.8746, 5);
				case 1: SetPlayerCheckpointEx(playerid, 2583.5859,2072.9514,10.8662, 5);
				case 2: SetPlayerCheckpointEx(playerid, 1426.4296,-1328.1350,13.5710, 5); 
				case 3: SetPlayerCheckpointEx(playerid, 2490.5774,-2485.4587,13.7320, 5);
				case 4: SetPlayerCheckpointEx(playerid, 1042.9701,-907.7836,42.6147, 5);
			}
		}	
		case 5: {
			// Stolen cars
			switch(rand) {
				case 0: SetPlayerCheckpointEx(playerid, 2831.0107,915.2982,10.9302, 5);
				case 1: SetPlayerCheckpointEx(playerid, 2327.8936,-2072.9585,13.5873, 5);
				case 2: SetPlayerCheckpointEx(playerid, 2386.1792,-1474.1869,23.8195, 5); 
				case 3: SetPlayerCheckpointEx(playerid, 2717.3513,-2224.1147,13.6100, 5);
				case 4: SetPlayerCheckpointEx(playerid, 1073.0592,1269.8995,10.8659, 5);
			}
		}					
	}
	UpdateJobStats(playerid);
	return 1;
}
Dialog:DIALOG_HELP(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new szDialog[1500];
	switch(listitem) {
		case 0: {
			strcat(szDialog, "Comenzi despre Cont:\n");
			strcat(szDialog, "/stats - verifici statisticile tale.\n");
			strcat(szDialog, "/changepass - iti schimbi parola.\n");
			strcat(szDialog, "/id - verifici ID-ul unui jucator.\n");
			strcat(szDialog, "/licenses - verifici licentele detinute.\n");
			strcat(szDialog, "/buylevel - iti cumperi nivel (daca ai punctele necesare).\n");
			strcat(szDialog, "/puthelmet - iti pui / scoti casca.\n");		
		}
		case 1: {
			strcat(szDialog, "Comenzi in General:\n");
			strcat(szDialog, "/report - poti raporta o problema.\n");
			strcat(szDialog, "/newbie - poti pune o intrebare.\n");
			strcat(szDialog, "/pay - dai bani unui jucator aflat langa tine.\n");
			strcat(szDialog, "/time - verifici minutele jucate de tine de la ultimul payday.\n");
			strcat(szDialog, "/buy - cumperi iteme din 24/7.\n");
			strcat(szDialog, "/drink - cumperi bautura dintr-un bar.\n");
			strcat(szDialog, "/showlicenses - arati licentele unui jucator.\n");
			strcat(szDialog, "/accept - accepti o invitatie/lucru.\n");
			strcat(szDialog, "/cancel - refuzi o invitatie/lucru.\n");
			strcat(szDialog, "/service - apelezi la un serviciu.\n");
			strcat(szDialog, "/eject - dai un player afara din vehicul.\n");
			strcat(szDialog, "/fill - faci plinu'.\n");
		}
		case 2: {
			strcat(szDialog, "Comenzi despre Chat:\n");
			strcat(szDialog, "/whisper - soptesti unui playeri aflat langa tine.\n");
			strcat(szDialog, "/shout - strigi.\n");
			strcat(szDialog, "/ad - pui un anunt.\n");
			strcat(szDialog, "/f - chatul factiunii.\n");
			strcat(szDialog, "/c - chatul clanului.\n");
			strcat(szDialog, "/me - spui o actiune.\n");
		}
		case 3: {
			strcat(szDialog, "Comenzi despre Banca:\n");
			strcat(szDialog, "/balance - vezi balanta contului tau.\n");
			strcat(szDialog, "/withdraw - scoti o suma de bani din contul tau bancar.\n");
			strcat(szDialog, "/deposit - pui o suma de bani in contul tau bancar.\n");
		}
		case 4: {
			if(PlayerInfo[playerid][pAdmin] == 0) return SCM(playerid, COLOR_WHITE, "{FF9494}* Nu esti admin.");
			strcat(szDialog, "Comenzi de Admin:\n");
			strcat(szDialog, "/a - chat intre admini.\n");
			strcat(szDialog, "/ah - comenzile disponibile.\n");
		}
		case 5: {
			strcat(szDialog, "Comenzi despre Vehicule:\n");
			strcat(szDialog, "/v - administrezi vehiculele personale.\n");
			strcat(szDialog, "/swapcolors - schimbi culorile intre ele.\n");
			strcat(szDialog, "/carplate - pui un numar de inmatriculare.\n");
			strcat(szDialog, "/park - parchezi vehiculul.\n");
			strcat(szDialog, "/carcolor - setezi culorile unui vehicul.\n");
			strcat(szDialog, "/lock (sau tasta N) - inchizi/deschizi vehiculul.\n");
			strcat(szDialog, "/sellcarto - vinzi vehiculul unui player.\n");
			strcat(szDialog, "/towcar - poti respawna un vehicul personal intr-un mod mai usor.\n");
			strcat(szDialog, "/findcar - poti localiza un vehicul personal intr-un mod mai usor.\n");										
		}
		case 6: {
			strcat(szDialog, "Comenzi despre Case:\n");
			strcat(szDialog, "/housemenu - meniul casei");
			strcat(szDialog, "/heal - iei viata din casa.\n");
			strcat(szDialog, "/furniture - mobilezi casa.\n");
			strcat(szDialog, "/editfurniture - editezi mobilerul.\n");			
			strcat(szDialog, "/sellhouseto - vinzi casa unui player.\n");
			strcat(szDialog, "/sellhousetostate - vinzi casa la stat.\n");			
		}
		case 7: {
			strcat(szDialog, "Comenzi despre Afaceri:\n");
			strcat(szDialog, "/bizmenu - meniul casei");
			strcat(szDialog, "/sellbiztostate - vinzi afacerea la stat.\n");		
			strcat(szDialog, "/sellbizto - vinzi afacearea unui jucator.\n");
		}
		case 8: {
			if(PlayerInfo[playerid][pMember] != 0)
			{
				if(PlayerInfo[playerid][pMember] == 1 || PlayerInfo[playerid][pMember] == 2 || PlayerInfo[playerid][pMember] == 3)
				{
					strcat(szDialog, "Comenzile factiunii PD:\n");
					strcat(szDialog, "/duty - te pui la datorie / iesi.\n");
					strcat(szDialog, "/mdc - verifica informatiile infractorului.\n");
					strcat(szDialog, "/su - dai wanted unui suspect.\n");
					strcat(szDialog, "/so - somezi un suspect.\n");
					strcat(szDialog, "/arrest - arestezi un suspect.\n");
					strcat(szDialog, "/tazer - blochezi un suspect.\n");
					strcat(szDialog, "/cuff - incatusezi un suspect.\n");
					strcat(szDialog, "/untie - dezlegi un player.\n");
					strcat(szDialog, "/uncuff - dezcatusezi un suspect.\n");
					strcat(szDialog, "/frisk - verifici suspectul de lucruri ilegale.\n");
					strcat(szDialog, "/clear - stergi wanted-ul unui player.\n");
					strcat(szDialog, "/d - chat intre departament.\n");
					strcat(szDialog, "/r - chat intre membri.\n");
					strcat(szDialog, "/wanted - vezi jucatorii ce au wanted.\n");
					if(PlayerInfo[playerid][pMember] == 1) strcat(szDialog, "/ticket - dai amenda unui suspect.\n");
					strcat(szDialog, "/confiscate - confisti un lucruri.\n");
					if(PlayerInfo[playerid][pMember] == 3) strcat(szDialog, "/punish - blochezi un player pentru un anumit timp.\n");
				}
				else if(PlayerInfo[playerid][pMember] == 4 || PlayerInfo[playerid][pMember] == 5 || PlayerInfo[playerid][pMember] == 6 || PlayerInfo[playerid][pMember] == 10)
				{
					strcat(szDialog, "Comenzile de Mafii:\n");
					strcat(szDialog, "/order - cumperi arme din HQ.\n");
					strcat(szDialog, "/attack - ataci un teritoriu.\n");
					strcat(szDialog, "/tie - legi un player.\n");
					strcat(szDialog, "/untie - dezlegi un player.\n");
					strcat(szDialog, "/stealers - lista cu cei ce au furat un vehicul al factiunii\n");			
				}
				else if(PlayerInfo[playerid][pMember] == 7)
				{
					strcat(szDialog, "Comenzile factiunii School Instructors:\n");
					strcat(szDialog, "/startlesson - incepi o lectie.\n");
					strcat(szDialog, "/stoplesson - opresti lectia.\n");	
					strcat(szDialog, "/givelicense - dai licenta unui player.\n");												
				}					
				else if(PlayerInfo[playerid][pMember] == 8)
				{
					strcat(szDialog, "Comenzile factiunii Tow Truck Company:\n");
					strcat(szDialog, "/tow - tractezi un vehicul.\n");
				}
				else if(PlayerInfo[playerid][pMember] == 9)
				{
					strcat(szDialog, "Comenzile factiunii News Reporters:\n");
					strcat(szDialog, "/news - dai anunturi.\n");
					strcat(szDialog, "/live - faci live cu un jucator.\n");
					strcat(szDialog, "/endlive - inchizi un live.\n");					
				}
				else if(PlayerInfo[playerid][pMember] == 11)
				{
					strcat(szDialog, "Comenzile factiunii Hitman:\n");
					strcat(szDialog, "/mycontract - vezi contractul tau.\n");
					strcat(szDialog, "/order - cumperi arme.\n");
					strcat(szDialog, "/gethit - iei un contract.\n");
					strcat(szDialog, "/leavehit - anulezi un contract.\n");
					strcat(szDialog, "/contracts - vezi contractele disponibile.\n");
					strcat(szDialog, "/undercover - te pui sub acoperire/iesi.\n");
					
				}
				else if(PlayerInfo[playerid][pMember] == 12)
				{
					strcat(szDialog, "Comenzile factiunii Taxi:\n");
					strcat(szDialog, "/fare - setezi tariful.\n");
					strcat(szDialog, "/accept taxi - accepti o comanda.\n");
				}
				else if(PlayerInfo[playerid][pMember] == 13)
				{
					strcat(szDialog, "Comenzile factiunii Paramedic:\n");
					strcat(szDialog, "/heal - dai viata unui jucator.\n");
					strcat(szDialog, "/d - chat intre departament.\n");
					strcat(szDialog, "/r - chat intre membri.\n");						
				}
				if(PlayerInfo[playerid][pMember] == 1 || PlayerInfo[playerid][pMember] == 2 || PlayerInfo[playerid][pMember] == 3) { }
				else {
					strcat(szDialog, "/f - chatul factiunii.\n");			 			
				}
				strcat(szDialog, "/fdeposit - depozitezi bani, materiale sau droguri.\n");					
			}
			else return SCM(playerid,COLOR_WHITE, "* Nu faci parte dintr-o factiune.");
		}
		case 9: {
			strcat(szDialog, "Comenzi ale Liderilor:\n");
			strcat(szDialog, "/invite - inviti un player.\n");
			strcat(szDialog, "/members - vezi membrii factiunii tale si poti sa te ocupi de cei membrii.\n");
			strcat(szDialog, "/l - chat intre lideri.\n");
			strcat(szDialog, "/setstatus - setezi statusul aplicatiilor.\n");
			strcat(szDialog, "/blacklist (/bl) - adaugi/scoti jucatorii de pe blacklist.\n");	
			strcat(szDialog, "/fwithdraw - scoti bani, materiale sau droguri din seif.\n");	
			strcat(szDialog, "/blockfc - opresti/activezi chatul factiunii.\n");	
			strcat(szDialog, "/lockhq - inchizi/deschizi HQ-ul\n");		
		}
		case 10: {
			strcat(szDialog, "Comenzi pentru Telefon:\n");
			strcat(szDialog, "/call - apelezi un jucator.\n");
			strcat(szDialog, "/sms - dai mesaj unui jucator.\n");
			strcat(szDialog, "/p - raspunzi la apel.\n");
			strcat(szDialog, "/h - inchizi apelul.\n");
			strcat(szDialog, "/number - vezi numarul unui jucator.\n");
		}
		case 11: {
			strcat(szDialog, "Comenzi generale pentru joburi:\n");
			strcat(szDialog, "/startwork - pornesti munca.\n");
			strcat(szDialog, "/stopwork - opresti munca.\n");
			strcat(szDialog, "/find - cauti un jucator.\n");
			strcat(szDialog, "/refill - alimentezi vehiculul unui jucator.\n");
			strcat(szDialog, "/repair - repari vehiculul unui jucator.\n");
			strcat(szDialog, "/sellmats - vinzi materiale.\n");
			strcat(szDialog, "/selldrugs - vinzi droguri.\n");
			strcat(szDialog, "/sellgun - vinzi arme.\n");
			strcat(szDialog, "/picklock - deschizi un vehicul incuiat.\n");
			strcat(szDialog, "/fish - incepi munca la job-ul pescar.\n");
		}
	}
	Dialog_Show(playerid, DIALOG_BACK, DIALOG_STYLE_MSGBOX, "Help", szDialog, "Back", "Cancel");
	return 1;
}	
Dialog:DIALOG_BACK(playerid, response, listitem, inputtext[]) {
	if(response) return Dialog_Show(playerid, DIALOG_HELP, DIALOG_STYLE_LIST, "Help", "Cont\nGeneral\nChat\nBanca\nAdmin\nVehicule\nCase\nBizz-uri\nFactiune\nLider\nTelefon\nJoburi\nPin", "Ok", "Exit");
	return 1;
}
Dialog:DIALOG_LOGIN2(playerid, response, listitem, inputtext[]) {
	if(!response) return Kick(playerid);
	if(strlen(inputtext)) {
		new tmppass[64];
		mysql_escape_string(inputtext, tmppass);
		OnPlayerLoginIn(playerid,MD5_Hash(tmppass));
	}
	else {
		new string[144]; 
		if(gPlayerLogTries[playerid] == 0)
			return KickEx(playerid);
		
		Dialog_Show(playerid, DIALOG_LOGIN3,DIALOG_STYLE_PASSWORD,"Logare", "Welcome to the "SERVER_NAME" RPG Server.\n\nPlease enter you password below!","Logare","Quit");
		gPlayerLogTries[playerid] --;				
		format(string, sizeof(string), "Incorrect password. You have %d remaining login attemps left.", gPlayerLogTries[playerid]);
		SCM(playerid, COLOR_WARNING, string);
	}
	return 1;
}
Dialog:DIALOG_LOGIN3(playerid, response, listitem, inputtext[]) {
	if(!response) return Kick(playerid);
	if(strlen(inputtext)) {
		new tmppass[64];
		mysql_escape_string(inputtext, tmppass);
		OnPlayerLoginIn(playerid,MD5_Hash(tmppass));
	}
	else {
		if(gPlayerLogTries[playerid] == 0) 
			return KickEx(playerid);

		new string[144]; 				
		Dialog_Show(playerid, DIALOG_LOGIN3,DIALOG_STYLE_PASSWORD,"Logare", "Welcome to the "SERVER_NAME" RPG Server.\n\nPlease enter you password below!","Logare","Quit");
		gPlayerLogTries[playerid] --;				
		format(string, sizeof(string), "Incorrect password. You have %d remaining login attemps left.", gPlayerLogTries[playerid]);
		SCM(playerid, COLOR_WARNING, string);
	}
	return 1;
}	
Dialog:DIALOG_LOGIN1(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new string[144]; 
	if(strlen(inputtext) >= 6 && strlen(inputtext) <= 30 && checker(inputtext) == 1) {
		new tmppass[64];
		mysql_escape_string(inputtext, tmppass);
		format(string, sizeof(string), "%s", formatpassword(inputtext));
		PlayerTextDrawSetString(playerid, RegisterPTD[1], string);
		format(gString, 50, "Your password contains %d characters.", strlen(inputtext));
		SCM(playerid, COLOR_YELLOW, gString);
		RegisterPass[playerid] = tmppass;
		RegisrationStep[playerid][3] = true;
	}
	else {
		format(string, sizeof(string), "Parola nesigura\n\nPentru a-ti creea unul, introdu o parola in casuta de mai jos.",GetName(playerid));
		SCM(playerid, COLOR_DARKNICERED, "Parola ta trebuie sa contina cel putin o litera mare si o cifra (minim 6, maxim 32 caractere).");
		Dialog_Show(playerid, DIALOG_LOGIN1, DIALOG_STYLE_PASSWORD, "Register", string, "Register", "Quit");
	}
	return 1;
}
Dialog:DIALOG_REGISTER(playerid, response, listitem, inputtext[]) {
	if(!response) 
		return true;

	RegisrationStep[playerid][0] = true;
	PlayerInfo[playerid][pSex] = listitem+1;
	PlayerTextDrawSetString(playerid, RegisterPTD[2], listitem ? "Female" : "Male");
	PlayerTextDrawSetPreviewModel(playerid, RegisterPTD[0], listitem ? 226 : 250);
	PlayerTextDrawShow(playerid, RegisterPTD[0]);
	format(gString, 50, "Gender set to: %s.", listitem ? "Female" : "Male");
	SCM(playerid, COLOR_YELLOW, gString);
	PlayerInfo[playerid][pChar] = listitem ? 226 : 250;
	return 1;
}
Dialog:DIALOG_REGISTER2(playerid, response, listitem, inputtext[]) {
	if(!response) 
		return true;

	if(strval(inputtext) < 7 || strval(inputtext) > 50) 
		return Dialog_Show(playerid, DIALOG_REGISTER2, DIALOG_STYLE_INPUT, "Register:", "Insert you character age:", "Select", "Close");
	
	PlayerInfo[playerid][pAge] = strval(inputtext);
	format(gString, 50, "Age set to: %d.", strval(inputtext));
	SCM(playerid, COLOR_YELLOW, gString);
	PlayerTextDrawSetString(playerid, RegisterPTD[5], inputtext);
	RegisrationStep[playerid][1] = true;
	return 1;
}
Dialog:DIALOG_REGISTER4(playerid, response, listitem, inputtext[]) {
	if(!response)
		return true;

	if(strlen(inputtext) > 40 || !IsMail(inputtext)) 
		return Dialog_Show(playerid, DIALOG_REGISTER4, DIALOG_STYLE_INPUT, "Register:", "Insert you email:\nInvalid.", "Select", "Close");

	new emailtext[64];
	mysql_escape_string(inputtext, emailtext);
	strmid(PlayerInfo[playerid][pEmail], emailtext, 0, strlen(emailtext), 64);
	RegisrationStep[playerid][2] = true;
	format(gString, 100, "Email set to: %s", PlayerInfo[playerid][pEmail]);
	SCM(playerid, COLOR_YELLOW, gString);
	PlayerTextDrawSetString(playerid, RegisterPTD[3], emailtext);
	return 1;
}
Dialog:DIALOG_REGISTER5(playerid, response, listitem, inputtext[]) {
	if(!response) 
		return true;
	
	if(!IsNumeric(inputtext))
		return Dialog_Show(playerid, DIALOG_REGISTER5, DIALOG_STYLE_INPUT, "Referral:", "Insert referral:", "Select", "Close");

	if(strval(inputtext) == PlayerInfo[playerid][pSQLID]) {
		SCM(playerid, COLOR_WARNING, "Nu iti poti pune ID-ul tau de referral!");
		Dialog_Show(playerid, DIALOG_REGISTER5, DIALOG_STYLE_INPUT, "Referral:", "Insert referral:", "Select", "Close");	
		return true;
	}
	gQuery[0] = EOS;
	mysql_format(SQL, gQuery, sizeof(gQuery), "SELECT * FROM `users` WHERE `id`='%d' LIMIT 1", strval(inputtext));
	mysql_tquery(SQL, gQuery, "SQL_CheckPlayer", "ii", playerid, strval(inputtext));
	return 1;
}	
Dialog:DIALOG_REGISTER6(playerid, response, listitem, inputtext[]) {
	if(!response) return Dialog_Show(playerid, DIALOG_REGISTER5, DIALOG_STYLE_INPUT, "Referral:", "Ai fost adus de cineva pe comunitate?\nDaca da, scrie ID-ul jucatorului care te-a adus.", "Ok", "Close");
	new id = GetPVarInt(playerid, "Referral");
	PlayerTextDrawSetString(playerid, RegisterPTD[4], RegisterReferral[playerid]);
	format(gString, 50, "Referral set to %s.", RegisterReferral[playerid]);
	SCM(playerid, COLOR_YELLOW, gString);
	foreach(new i: Player) {
		if(PlayerInfo[i][pSQLID] == id) {
			new string[144];
			format(string, sizeof(string), "Iti multumim ca l-ai adus pe %s(%d) pe comunitate!", GetName(playerid), playerid);
			SCM(i, COLOR_MONEY, string);
		}
	}
	return 1;
}
Dialog:DIALOG_TRANSFER(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(TradeID[playerid] != -1) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu poti folosi aceasta comanda atata timp cat esti implicat intr-o afacere!");
	new cashbank = TransferMoney[playerid]/100;
	if(PlayerInfo[playerid][pStoreAccount] == 0 && PlayerInfo[playerid][pAccount] < TransferMoney[playerid]+cashbank) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu ai suma necesara pentru a transfera banii!");
	new LastMoney[50], string[144];
	format(LastMoney, 50, GetBankMoney(playerid));
	GivePlayerBank(playerid, -TransferMoney[playerid]+cashbank);
	GivePlayerBank(TransferOffer[playerid], TransferMoney[playerid]);
	Update(TransferOffer[playerid], pBankx);
	Update(TransferOffer[playerid], pMBankx);
	Update(playerid, pBankx);
	Update(playerid, pMBankx);
	BizzInfo[InBussines[playerid]][bBalance] += cashbank;
	format(string, sizeof(string), "Ai transferat $%s in contul bancar al lui %s. Taxa: $%s", FormatNumber(TransferMoney[playerid]), GetName(TransferOffer[playerid]),FormatNumber(cashbank));
	PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
	SCM(playerid, COLOR_MONEY, string);
	format(string, sizeof(string), "Ai primit $%s in contul tau bancar de la %s.", FormatNumber(TransferMoney[playerid]), GetName(playerid), playerid);
	SCM(TransferOffer[playerid], COLOR_MONEY, string);
	if(PlayerInfo[TransferOffer[playerid]][pShowBanca] == 1) {
		format(string, sizeof(string), "$%s", GetBankMoney(TransferOffer[playerid]));
		PlayerTextDrawSetString(TransferOffer[playerid], BancaTD, string);
	}
	if(PlayerInfo[playerid][pShowBanca] == 1) {
		format(string, sizeof(string), "$%s", GetBankMoney(playerid));
		PlayerTextDrawSetString(playerid, BancaTD, string);
	}
	format(string, sizeof(string), "($) %s a transferat $%s lui %s, [last bank money: $%s]", GetName(playerid), FormatNumber(TransferMoney[playerid]), GetName(TransferOffer[playerid]), LastMoney);
	SendAdminMessage(COLOR_YELLOW, string, 1);
	
	new query[144];
	PlayerPlaySound(TransferOffer[playerid], 1052, 0.0, 0.0, 0.0);
	mysql_format(SQL, query,sizeof(query),"UPDATE `bizz` SET `Till`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance], InBussines[playerid]);
	mysql_tquery(SQL,query, "", "");
	new ip1[42],ip2[42];
	GetPlayerIp(playerid,ip1,sizeof(ip1));
	GetPlayerIp(TransferOffer[playerid],ip2,sizeof(ip2));
	Transferlog(ip2,ip1,PlayerInfo[TransferOffer[playerid]][pSQLID],PlayerInfo[playerid][pSQLID],TransferMoney[playerid]);
	TransferOffer[playerid] = 999;
	TransferMoney[playerid] = 0;
	return 1;
}
Dialog:DIALOG_NEWPASS(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(strcmp(MD5_Hash(inputtext), PlayerInfo[playerid][pKey])) {
		SCM(playerid, COLOR_WARNING, "Parola invalida!");
		return true;
	}
	Dialog_Show(playerid, DIALOG_NEWPASS1,DIALOG_STYLE_INPUT, "Change password","Scrie mai jos noua parola pe care vrei s-o ai.","Ok","Close");
	return 1;
}
Dialog:DIALOG_NEWPASS1(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new length = strlen(inputtext);
	if(length == 0 || length > 15) return Dialog_Show(playerid, DIALOG_NEWPASS1,DIALOG_STYLE_INPUT, "Change password","Scrie mai jos noua parola pe care vrei s-o ai.","Ok","Close");
	if(strlen(inputtext) > 1 && strlen(inputtext) < 16) {
		new emailtext[64], string[144];
		mysql_escape_string(inputtext, emailtext);
		format(PlayerInfo[playerid][pKey], 50, MD5_Hash(emailtext));
		new str1[512];
		mysql_format(SQL, str1,512,"UPDATE users SET `password`='%s' WHERE `ID`='%d'",PlayerInfo[playerid][pKey],PlayerInfo[playerid][pSQLID]);
		mysql_tquery(SQL,str1, "", "");
		format(string, sizeof(string), "Noua ta parola este %s.", inputtext);
		Dialog_Show(playerid, 0, DIALOG_STYLE_MSGBOX, "New password", string, "Ok", "");
		format(string, sizeof(string), "Ti-ai schimbat parola!");
		SCM(playerid, COLOR_YELLOW,string);
	}
	else Dialog_Show(playerid, DIALOG_NEWPASS1,DIALOG_STYLE_INPUT, "Schimba parola:","Scrie parola pe care  vrei s-o ai.","Ok","Iesi");
	return 1;
}
Dialog:DIALOG_LOCATIONS(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(CP[playerid] != 0) return Dialog_Show(playerid, DIALOG_CHECKPOINT, DIALOG_STYLE_MSGBOX, "Checkpoint", "Ai deja un checkpoint activ.\nDoresti sa-l anulezi? Daca da, apasa pe 'Ok'.", "Ok", "Exit");
	switch(listitem) {
		case 0: {
			new string[400];
			for(new i = 0; i < 18; i++) {
				format(string, sizeof(string), "[# %d]. %s (%d)\n", i+1, BizzName[i], GpsBizzType[i]);
				strcat(gString, string);
			}
			Dialog_Show(playerid, DIALOG_LOCATIONS3, DIALOG_STYLE_LIST, "Busines-uri", gString, "Select", "Exit");			
		}
		case 1: {
			new szDialog[1024], szDialog2[1024];
			format(szDialog, 1024, "Dealership - %0.2fm\n", GetPlayerDistanceFromPoint(playerid, 1487.0498,-2287.2930,13.7529));
			strcat(szDialog2, szDialog);
			format(szDialog, 1024, "Place License - %0.2fm\n", GetPlayerDistanceFromPoint(playerid, 2333.2207,-18.0495,26.4844));
			strcat(szDialog2, szDialog);
			format(szDialog, 1024, "Giftbox - %0.2fm\n", GetPlayerDistanceFromPoint(playerid, 2239.6196,566.6007,10.8815));
			strcat(szDialog2, szDialog);
			format(szDialog, 1024, "Arena de evente - %0.2fm\n", GetPlayerDistanceFromPoint(playerid, 1671.9180,-1328.6810,17.4482));
			strcat(szDialog2, szDialog);
			format(szDialog, 1024, "Stage System & Neaone- %0.2fm\n", GetPlayerDistanceFromPoint(playerid, 1568.0173,-2177.0515,13.5765));
			strcat(szDialog2, szDialog);	
			Dialog_Show(playerid, DIALOG_LOCATIONS2, DIALOG_STYLE_LIST, "Locatii", szDialog2, "Select", "Exit");			
		}
		case 2: {
			new szDialog[1024], szDialog2[1024];
			format(szDialog, 1024, "Tunning Central LS - %0.2fm\n", GetPlayerDistanceFromPoint(playerid, 1041.3690,-1028.5861,32.1016));
			strcat(szDialog2, szDialog);
			format(szDialog, 1024, "Tunning Willowfield LS - %0.2fm\n", GetPlayerDistanceFromPoint(playerid, 2644.8350,-2016.4033,13.5540));
			strcat(szDialog2, szDialog);
			format(szDialog, 1024, "Tunning LV - %0.2fm\n", GetPlayerDistanceFromPoint(playerid, 2387.1628,1039.3945,10.820));
			strcat(szDialog2, szDialog);
			format(szDialog, 1024, "Tunning SF - %0.2fm\n", GetPlayerDistanceFromPoint(playerid, -1935.4766,235.7379,34.3125));
			strcat(szDialog2, szDialog);
			format(szDialog, 1024, "Tunning Arch Wheels SF - %0.2fm\n", GetPlayerDistanceFromPoint(playerid, -2713.5532,217.8432,4.2647));
			strcat(szDialog2, szDialog);
			Dialog_Show(playerid, DIALOG_LOCATIONS4, DIALOG_STYLE_LIST, "Tunnings Locations", szDialog2, "Select", "Exit");			
		}	
		case 3: {
			CP[playerid] = 43;
			SetPlayerCheckpointEx(playerid, 154.9603,-1950.8998,6.0030, 20.0);
			SendClientMessage(playerid, COLOR_YELLOW, "Ti-a fost pus un checkpoint catre locatia 'crates & Gift box' aleasa de tine!");
		}
	}	
	return 1;
}
Dialog:DIALOG_LOCATIONS2(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(CP[playerid] != 0) return Dialog_Show(playerid, DIALOG_CHECKPOINT, DIALOG_STYLE_MSGBOX, "Checkpoint", "Ai deja un checkpoint activ.\nDoresti sa-l anulezi? Daca da, apasa pe 'Ok'.", "Ok", "Exit");
	switch(listitem) {
		case 0: SetPlayerCheckpointEx(playerid, 1487.0498,-2287.2930,13.7529, 2.0);//dealership
		case 1: SetPlayerCheckpointEx(playerid, 2333.2207,-18.0495,26.4844, 2.0);
		case 2: SetPlayerCheckpointEx(playerid, 2239.6196,566.6007,10.8815, 2.0);	// giftbox		
		case 3: SetPlayerCheckpointEx(playerid, 1671.9180,-1328.6810,17.4482, 2.0);
		case 4: SetPlayerCheckpointEx(playerid, 1568.0173,-2177.0515,13.5765, 40.0);
	}
	CP[playerid] = 43;
	SendClientMessage(playerid, COLOR_YELLOW, "Ti-a fost pus un checkpoint catre locatia importanta aleasa de tine!");
	return 1;
}
Dialog:DIALOG_LOCATIONS3(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(CP[playerid] != 0) return Dialog_Show(playerid, DIALOG_CHECKPOINT, DIALOG_STYLE_MSGBOX, "Checkpoint", "Ai deja un checkpoint activ.\nDoresti sa-l anulezi? Daca da, apasa pe 'Ok'.", "Ok", "Exit");
	for(new i = 1; i <= GpsBizzType[listitem]; i++) {
		new idd = BizzTypeID[listitem][i];
		format(gString, sizeof(gString), "%s%s (%d) - %0.2fm\n", gString, BizzName[listitem], BizzInfo[idd][bID], GetPlayerDistanceFromPoint(playerid, BizzInfo[idd][bEntranceX], BizzInfo[idd][bEntranceY], BizzInfo[idd][bEntranceZ]));
	}
	Dialog_Show(playerid, DIALOG_LOCATIONS5, DIALOG_STYLE_LIST, "Busines-uri", gString, "Select", "Exit");		
	listitemm[playerid] = listitem;
	return 1;
}
Dialog:DIALOG_LOCATIONS4(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(CP[playerid] != 0) return Dialog_Show(playerid, DIALOG_CHECKPOINT, DIALOG_STYLE_MSGBOX, "Checkpoint", "Ai deja un checkpoint activ.\nDoresti sa-l anulezi? Daca da, apasa pe 'Ok'.", "Ok", "Exit");
	switch(listitem) {
		case 0: SetPlayerCheckpointEx(playerid, 1041.3690,-1028.5861,32.1016, 2.0);
		case 1: SetPlayerCheckpointEx(playerid, 2644.8350,-2016.4033,13.5540, 2.0);			
		case 2: SetPlayerCheckpointEx(playerid, 2387.1628,1039.3945,10.820, 5.0);
		case 3: SetPlayerCheckpointEx(playerid, -1935.4766,235.7379,34.3125, 5.0);
		case 4: SetPlayerCheckpointEx(playerid, -2713.5532,217.8432,4.2647, 5.0);		
	}
	CP[playerid] = 43;
	SendClientMessage(playerid, COLOR_YELLOW, "Ti-a fost pus un checkpoint catre tunningul ales de tine!");
	return 1;
}
Dialog:DIALOG_LOCATIONS5(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	if(CP[playerid] != 0) return Dialog_Show(playerid, DIALOG_CHECKPOINT, DIALOG_STYLE_MSGBOX, "Checkpoint", "Ai deja un checkpoint activ.\nDoresti sa-l anulezi? Daca da, apasa pe 'Ok'.", "Ok", "Exit");
	new idd = BizzTypeID[listitemm[playerid]][listitem+1];
	SetPlayerCheckpointEx(playerid, BizzInfo[idd][bEntranceX], BizzInfo[idd][bEntranceY], BizzInfo[idd][bEntranceZ], 2.0);		
	CP[playerid] = 43;
	SendClientMessage(playerid, COLOR_YELLOW, "Ti-a fost pus un checkpoint catre bizz-ul ales de tine!");
	return 1;
}
Dialog:DIALOG_STORE(playerid, response, listitem, inputtext[]) {
	if(!response) return true;
	new string[250], query[250];
	switch(listitem) {
		case 0: { 
			if(PlayerMoney(playerid, 1000)) return SCM(playerid, COLOR_GREY, "Nu ai destui bani pentru a face asta.");
			new tel[16];
			format(tel, 256, "%d", PlayerInfo[playerid][pPhone]);								
			if(strlen(tel) == 4) return SCM(playerid, COLOR_GREY, "Nu poti cumpara un alt numar de telefon deoarece ai iPhone!");
			GivePlayerCash(playerid, 0,1000);
			BizzInfo[InBussines[playerid]][bBalance] += 1000;				
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
			new randphone = 5000 + random(9999) + 5000 ;
			PlayerInfo[playerid][pPhone] = randphone;
			format(string, sizeof(string), "Ti-ai cumparat un telefon iar numarul tau este %d.", randphone);
			SCM(playerid, COLOR_WHITE, string);
			mysql_format(SQL, query,sizeof(query),"UPDATE users SET `Money`='%d', `MStore`='%d',`PhoneNr`='%d' WHERE `ID`='%d'",MoneyMoney[playerid],StoreMoney[playerid], PlayerInfo[playerid][pPhone],PlayerInfo[playerid][pSQLID]);
			mysql_tquery(SQL,query, "", "");
			mysql_format(SQL, query,sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL,query, "", "");
			if(PlayerInfo[playerid][pTutorial] == 5) RegisrationStep[playerid][0] = false;
		}
		case 1: {
			if(PlayerMoney(playerid, 750)) return SCM(playerid, COLOR_GREY, "Nu ai destui bani pentru a face asta.");
			GivePlayerCash(playerid, 0,750);
			BizzInfo[InBussines[playerid]][bBalance] += 750;
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
			PlayerInfo[playerid][pPhoneBook] = 1;
			SCM(playerid, COLOR_WHITE, "Ti-ai cumparat o agenda telefonica. Foloseste comanda /number pentru a vedea numarul unui player.");
			mysql_format(SQL, query,sizeof(query),"UPDATE users SET `Money`='%d',`MStore`='%d', `Phonebook`='%d' WHERE `ID`='%d'",MoneyMoney[playerid],StoreMoney[playerid], PlayerInfo[playerid][pPhoneBook],PlayerInfo[playerid][pSQLID]);
			mysql_tquery(SQL,query, "", "");
			mysql_format(SQL, query,sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL,query, "", "");
			if(PlayerInfo[playerid][pTutorial] == 5) RegisrationStep[playerid][1] = false;
		}
		case 2: {
			if(PlayerMoney(playerid, 750)) return SCM(playerid, COLOR_GREY, "Nu ai destui bani pentru a face asta.");
			GivePlayerCash(playerid, 0,500);
			BizzInfo[InBussines[playerid]][bBalance] += 500;
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
			PlayerInfo[playerid][pCredit] += 25;
			SCM(playerid, COLOR_WHITE, "Ti-ai cumparat o cartela de credit de 25$ pentru suma de $500.");
			mysql_format(SQL, query,sizeof(query),"UPDATE users SET `Money`='%d',`MStore`='%d', `Credit`='%d' WHERE `ID`='%d'",MoneyMoney[playerid],StoreMoney[playerid], PlayerInfo[playerid][pCredit],PlayerInfo[playerid][pSQLID]);
			mysql_tquery(SQL,query, "", "");
			mysql_format(SQL, query,sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL,query, "", "");
			if(PlayerInfo[playerid][pTutorial] == 5) RegisrationStep[playerid][2] = false;
		}
		case 3: {
			if(PlayerMoney(playerid, 1500)) return SCM(playerid, COLOR_GREY, "Nu ai destui bani pentru a face asta.");
			GivePlayerCash(playerid, 0, 1500);
			BizzInfo[InBussines[playerid]][bBalance] += 1500;			
			PlayerInfo[playerid][pWTalkie] = 1;
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
			SCM(playerid, COLOR_WHITE, "Ti-ai cumparat o frecventa radio. Foloseste /setfreq pentru a seta frecventa.");
			format(query,sizeof(query),"UPDATE users SET `Money`='%d', `MStore`='%d', `WTalkie`='1' WHERE `ID`='%d'",MoneyMoney[playerid],StoreMoney[playerid], PlayerInfo[playerid][pSQLID]);
			mysql_query(SQL,query);
			mysql_format(SQL,query,sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL,query, "", "");
		}
		case 4: {
			if(PlayerMoney(playerid, 500)) return SCM(playerid, COLOR_GREY, "Nu ai destui bani pentru a face asta.");
			if(PlayerInfo[playerid][pLighter] != 0) return SCM(playerid, COLOR_WHITE, "Ai deja o bricheta.");
			GivePlayerCash(playerid, 0, 500);
			BizzInfo[InBussines[playerid]][bBalance] += 500;			
			PlayerInfo[playerid][pLighter] = 10;
			SCM(playerid, -1, "Ti-ai cumparat o bricheta.");
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
			Update(playerid, pCashx);
			mysql_format(SQL,query,sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL,query, "", "");
		}
		case 5: {
			if(PlayerMoney(playerid, 100)) return SCM(playerid, COLOR_GREY, "Nu ai destui bani pentru a face asta.");
			if(PlayerInfo[playerid][pCigarettes] != 0) return SCM(playerid, COLOR_WHITE, "Ai deja tigari.");
			GivePlayerCash(playerid, 0, 100);
			BizzInfo[InBussines[playerid]][bBalance] += 100;			
			PlayerInfo[playerid][pCigarettes] = 10;
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
			SCM(playerid, COLOR_WHITE, "Ti-ai cumparat tigari. Ca sa fumezi, foloseste comanda /smoke.");
			Update(playerid, pCashx);
			mysql_format(SQL,query,sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL,query, "", "");
		}
		case 6: {
			if(PlayerMoney(playerid, 2000)) return SCM(playerid, COLOR_GREY, "Nu ai destui bani pentru a face asta.");
			if(PlayerInfo[playerid][pGasCan] != 0) return SCM(playerid, COLOR_WHITE, "Ai deja canistra.");
			GivePlayerCash(playerid, 0, 2000);
			BizzInfo[InBussines[playerid]][bBalance] += 2000;				
			PlayerInfo[playerid][pGasCan] = 1;
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
			SCM(playerid, COLOR_WHITE, "Ti-ai cumparat o canistra. Pentru a pune benzina in canistra, foloseste /fillgascan.");
			mysql_format(SQL, query,sizeof(query),"UPDATE users SET `Money`='%d', `MStore`='%d', `GasCan`='1' WHERE `ID`='%d'",MoneyMoney[playerid],StoreMoney[playerid], PlayerInfo[playerid][pSQLID]);
			mysql_tquery(SQL,query, "", "");
			mysql_format(SQL,query,sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL,query, "", "");
		}
		case 7: {
			if(PlayerMoney(playerid, 3000)) return SCM(playerid, COLOR_GREY, "Nu ai destui bani pentru a face asta.");
			GivePlayerCash(playerid, 0, 3000);
			BizzInfo[InBussines[playerid]][bBalance] += 3000;				
			ServerWeapon(playerid, 43, 50);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
			SCM(playerid, COLOR_WHITE, "Ti-ai cumparat o camera foto pentru $3,000.");
			Update(playerid, pCashx);
			mysql_format(SQL,query,sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL,query, "", "");
		}
		case 8: {
			if(PlayerMoney(playerid, 6000)) return SCM(playerid, COLOR_GREY, "Nu ai destui bani pentru a face asta.");
			GivePlayerCash(playerid, 0, 6000);
			BizzInfo[InBussines[playerid]][bBalance] += 6000;				
			ServerWeapon(playerid, 41, 500);
			PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
			SCM(playerid, COLOR_WHITE, "Ti-ai cumparat o Spraycan pentru $6,000.");
			mysql_format(SQL, query,sizeof(query),"UPDATE users SET `Money`='%d', `MStore`='%d', WHERE `ID`='%d'",MoneyMoney[playerid],StoreMoney[playerid], PlayerInfo[playerid][pSQLID]);
			mysql_tquery(SQL,query, "", "");
			mysql_format(SQL,query,sizeof(query),"UPDATE `bizz` SET `Till`='%d',`Products`='%d' WHERE `ID`='%d'",BizzInfo[InBussines[playerid]][bBalance],BizzInfo[InBussines[playerid]][bProducts],InBussines[playerid]);
			mysql_tquery(SQL,query, "", "");
		}
	}
	if(PlayerInfo[playerid][pTutorial] == 5) {
		if(PlayerInfo[playerid][pPhone] > 0) {
			if(PlayerInfo[playerid][pPhoneBook] == 1) {
				if(PlayerInfo[playerid][pCredit] >= 25) {
					Tutorial(playerid);
				}
			}
		}
	}
	return 1;
}	