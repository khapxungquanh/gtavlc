
#include <YSI_Coding\y_hooks>

#define MAX_AOBJECTS 150

#define DIALOG_ACCS	809
#define DIALOG_BUY_ACCS 810
#define DIALOG_EDITACCS 811

#define SAVE_ACCESORY 0
#define SAVE_BONE 1
#define SAVE_STATUS 2
#define SAVE_INDEX_0 3
#define SAVE_INDEX_1 4
#define SAVE_INDEX_2 5
#define SAVE_INDEX_3 6
#define SAVE_INDEX_4 7
#define SAVE_INDEX_5 8
#define SAVE_INDEX_6 9
#define SAVE_INDEX_7 10
#define SAVE_INDEX_8 11
#define SAVE_INDEX_9 12

enum objects_attach_Info {
	objID,
	objModel,
	objType,	
	objMoney,
	objPremium,
	objBone,
	Float: objfOffsetX, 
	Float: objfOffsetY, 
	Float: objfOffsetZ, 
	Float: objfRotX, 
	Float: objfRotY, 
	Float: objfRotZ
};

enum attached_object_data
{
    ao_model,
    ao_bone,
    Float:ao_x,
    Float:ao_y,
    Float:ao_z,
    Float:ao_rx,
    Float:ao_ry,
    Float:ao_rz,
    Float:ao_sx,
    Float:ao_sy,
    Float:ao_sz
}
new ao[MAX_PLAYERS][MAX_PLAYER_ATTACHED_OBJECTS][attached_object_data];

enum player_object_Info {
	aID[MAX_PLAYER_ATTACHED_OBJECTS],
	aModel[MAX_PLAYER_ATTACHED_OBJECTS],
	aBone[MAX_PLAYER_ATTACHED_OBJECTS],
	aStatus[MAX_PLAYER_ATTACHED_OBJECTS],
	Float: aIndex0[6],
	Float: aIndex1[6],
	Float: aIndex2[6],
	Float: aIndex3[6],
	Float: aIndex4[6],
	Float: aIndex5[6],
	Float: aIndex6[6],
	Float: aIndex7[6],
	Float: aIndex8[6],
	Float: aIndex9[6]
};

new
	total_objects,
	objectInfo[MAX_AOBJECTS][objects_attach_Info],
	attachInfo[MAX_PLAYERS][player_object_Info],
	ObjectSlot[MAX_PLAYERS][100],
	ObjectID[MAX_PLAYERS],
	ObjectType[MAX_PLAYERS],
	Text: AccsTD[13],
	PlayerText: AccsPTD[3];

hook OnGameModeInit() load_accs_td(), SetTimer("load_objects_accesory", 1000, false);
hook OnPlayerConnect(playerid) load_accs_ptd(playerid), SetPVarInt(playerid, "accs", 0);
hook OnPlayerSpawn(playerid) attach_objects(playerid);
SetPlayerAttachedObjectEx(playerid, index, modelid, bone, Float:fOffsetX = 0.0, Float:fOffsetY = 0.0, Float:fOffsetZ = 0.0, Float:fRotX = 0.0, Float:fRotY = 0.0, Float:fRotZ = 0.0, Float:fScaleX = 1.0, Float:fScaleY = 1.0, Float:fScaleZ = 1.0, materialcolor1 = 0, materialcolor2 = 0)
{
    ao[playerid][index][ao_x] = fOffsetX;
    ao[playerid][index][ao_y] = fOffsetY;
    ao[playerid][index][ao_z] = fOffsetZ;
    ao[playerid][index][ao_rx] = fRotX;
    ao[playerid][index][ao_ry] = fRotY;
    ao[playerid][index][ao_rz] = fRotZ;
    ao[playerid][index][ao_sx] = fScaleX;
    ao[playerid][index][ao_sy] = fScaleY;
    ao[playerid][index][ao_sz] = fScaleZ;
    SetPlayerAttachedObject(playerid, index, modelid, bone, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ, materialcolor1, materialcolor2);
    return 1;
}

public OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ)
{
    if(response)
    {
    	SetPVarInt(playerid, "Edited", 1);
    	new type = ObjectType[playerid];
		switch(type) {
			case 0: {
				attachInfo[playerid][aIndex0][0] = fOffsetX; attachInfo[playerid][aIndex0][1] = fOffsetY; attachInfo[playerid][aIndex0][2] = fOffsetZ;
				attachInfo[playerid][aIndex0][3] = fRotX; attachInfo[playerid][aIndex0][4] = fRotY; attachInfo[playerid][aIndex0][5] = fRotZ;
				save_player_accs(playerid, SAVE_INDEX_0);
			}
			case 1: {
				attachInfo[playerid][aIndex1][0] = fOffsetX; attachInfo[playerid][aIndex1][1] = fOffsetY; attachInfo[playerid][aIndex1][2] = fOffsetZ;
				attachInfo[playerid][aIndex1][3] = fRotX; attachInfo[playerid][aIndex1][4] = fRotY; attachInfo[playerid][aIndex1][5] = fRotZ;
				save_player_accs(playerid, SAVE_INDEX_1);
			}
			case 2: {
				attachInfo[playerid][aIndex2][0] = fOffsetX; attachInfo[playerid][aIndex2][1] = fOffsetY; attachInfo[playerid][aIndex2][2] = fOffsetZ;
				attachInfo[playerid][aIndex2][3] = fRotX; attachInfo[playerid][aIndex2][4] = fRotY; attachInfo[playerid][aIndex2][5] = fRotZ;
				save_player_accs(playerid, SAVE_INDEX_2);
			}
			case 3: {
				attachInfo[playerid][aIndex3][0] = fOffsetX; attachInfo[playerid][aIndex3][1] = fOffsetY; attachInfo[playerid][aIndex3][2] = fOffsetZ;
				attachInfo[playerid][aIndex3][3] = fRotX; attachInfo[playerid][aIndex3][4] = fRotY; attachInfo[playerid][aIndex3][5] = fRotZ;
				save_player_accs(playerid, SAVE_INDEX_3);
			}
			case 4: {
				attachInfo[playerid][aIndex4][0] = fOffsetX; attachInfo[playerid][aIndex4][1] = fOffsetY; attachInfo[playerid][aIndex4][2] = fOffsetZ;
				attachInfo[playerid][aIndex4][3] = fRotX; attachInfo[playerid][aIndex4][4] = fRotY; attachInfo[playerid][aIndex4][5] = fRotZ;
				save_player_accs(playerid, SAVE_INDEX_4);
			}
			case 5: {
				attachInfo[playerid][aIndex5][0] = fOffsetX; attachInfo[playerid][aIndex5][1] = fOffsetY; attachInfo[playerid][aIndex5][2] = fOffsetZ;
				attachInfo[playerid][aIndex5][3] = fRotX; attachInfo[playerid][aIndex5][4] = fRotY; attachInfo[playerid][aIndex5][5] = fRotZ;
				save_player_accs(playerid, SAVE_INDEX_5);
			}
			case 6: {
				attachInfo[playerid][aIndex6][0] = fOffsetX; attachInfo[playerid][aIndex6][1] = fOffsetY; attachInfo[playerid][aIndex6][2] = fOffsetZ;
				attachInfo[playerid][aIndex6][3] = fRotX; attachInfo[playerid][aIndex6][4] = fRotY; attachInfo[playerid][aIndex6][5] = fRotZ;
				save_player_accs(playerid, SAVE_INDEX_6);
			}
			case 7: {
				attachInfo[playerid][aIndex7][0] = fOffsetX; attachInfo[playerid][aIndex7][1] = fOffsetY; attachInfo[playerid][aIndex7][2] = fOffsetZ;
				attachInfo[playerid][aIndex7][3] = fRotX; attachInfo[playerid][aIndex7][4] = fRotY; attachInfo[playerid][aIndex7][5] = fRotZ;
				save_player_accs(playerid, SAVE_INDEX_7);
			}
			case 8: {
				attachInfo[playerid][aIndex8][0] = fOffsetX; attachInfo[playerid][aIndex8][1] = fOffsetY; attachInfo[playerid][aIndex8][2] = fOffsetZ;
				attachInfo[playerid][aIndex8][3] = fRotX; attachInfo[playerid][aIndex8][4] = fRotY; attachInfo[playerid][aIndex8][5] = fRotZ;
				save_player_accs(playerid, SAVE_INDEX_8);
			}
			case 9: {
				attachInfo[playerid][aIndex9][0] = fOffsetX; attachInfo[playerid][aIndex9][1] = fOffsetY; attachInfo[playerid][aIndex9][2] = fOffsetZ;
				attachInfo[playerid][aIndex9][3] = fRotX; attachInfo[playerid][aIndex9][4] = fRotY; attachInfo[playerid][aIndex9][5] = fRotZ;
				save_player_accs(playerid, SAVE_INDEX_9);
			}								
		}
        ao[playerid][index][ao_x] = fOffsetX;
        ao[playerid][index][ao_y] = fOffsetY;
        ao[playerid][index][ao_z] = fOffsetZ;
        ao[playerid][index][ao_rx] = fRotX;
        ao[playerid][index][ao_ry] = fRotY;
        ao[playerid][index][ao_rz] = fRotZ;
        ao[playerid][index][ao_sx] = fScaleX;
        ao[playerid][index][ao_sy] = fScaleY;
        ao[playerid][index][ao_sz] = fScaleZ;	
    }
    else
    {
        new i = index;
        SetPlayerAttachedObjectEx(playerid, index, modelid, boneid, ao[playerid][i][ao_x], ao[playerid][i][ao_y], ao[playerid][i][ao_z], ao[playerid][i][ao_rx], ao[playerid][i][ao_ry], ao[playerid][i][ao_rz], ao[playerid][i][ao_sx], ao[playerid][i][ao_sy], ao[playerid][i][ao_sz]);
        printf("SetPlayerAttachedObject(playerid, %d, %d, %d, %f, %f, %f, %f, %f, %f, %f, %f, %f)", index, modelid, boneid, ao[playerid][i][ao_x], ao[playerid][i][ao_y], ao[playerid][i][ao_z], ao[playerid][i][ao_rx], ao[playerid][i][ao_ry], ao[playerid][i][ao_rz], ao[playerid][i][ao_sx], ao[playerid][i][ao_sy], ao[playerid][i][ao_sz]);
    }
    return 1;
}

YCMD:edit(playerid, params[], help) {
	new string[512], string2[512];
	if(GetPVarInt(playerid, "accs") == 1) {
		if(PlayerInfo[playerid][pAdmin] == 0) return SCM(playerid, COLOR_WARNING, "Eroare: {ffffff}Nu poti folosi aceasta comanda acum!");
		new price, pp;
		if(sscanf(params, "ii", price, pp)) return SendClientMessage(playerid, -1, "(ADMIN) /edit <price> <premium points>");
		new i = ObjectSlot[playerid][ObjectID[playerid]];

		objectInfo[i][objPremium] = pp;
		objectInfo[i][objMoney] = price;

		foreach(new x: Player) {
			if(IsPlayerConnected(x) && GetPVarInt(x, "accs") == 1) update_accs(playerid);
		}
		
		new query[128];
		format(query, sizeof(query), "UPDATE `objects_accesory` SET `Money`='%d', `Premium`='%d' WHERE `ID`='%d'", price, pp, i);
		mysql_tquery(SQL, query, "", "");
		return 1;
	}	
	strcat(string, "Category\tModel\tStatus\n");
	for(new i = 0; i < 8; i++) {
		if(attachInfo[playerid][aModel][i] != 0) format(string2, sizeof(string2), "%s\t%d\t%s{ffffff}\n", categoryName(i), attachInfo[playerid][aModel][i], (attachInfo[playerid][aStatus][i] > 0) ? ("{D60404}disabled") : ("{10B807}activated"));
		else format(string2, sizeof(string2), "%s\tnone\tunavailable\n", categoryName(i));
		strcat(string, string2);
	}
	strcat(string, "{D60404}Delete all");
    ShowPlayerDialog(playerid, DIALOG_EDITACCS, DIALOG_STYLE_TABLIST_HEADERS, "SERVER: Edit clothes", string, "Select", "Cancel");
    return 1;				
}

stock categoryName(type) {
	new string[32];
	switch(type) {
		case 0: string = "Hats";
		case 1: string = "Glasses";
		case 2: string = "Watches";
		case 3: string = "Lasers";
		case 4: string = "Hand items";
		case 5: string = "Masks & Gang Bandanas";
		case 6: string = "Back Items";
		case 7: string = "Special Items";
	}
	return string;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid) {
	if(clickedid == AccsTD[4]) {
		new 
			string[128], string2[128],
			id = ObjectSlot[playerid][ObjectID[playerid]];

		strcat(string, "Payment\tAmount\n");
		format(string2, sizeof(string2), "Money\t$%s\n", FormatNumber(objectInfo[id][objMoney])), strcat(string, string2);
		if(objectInfo[id][objPremium] != 0) format(string2, sizeof(string2), "diamante\t%s", FormatNumber(objectInfo[id][objPremium])), strcat(string, string2), strcat(string, string2);
			
	    ShowPlayerDialog(playerid, DIALOG_BUY_ACCS, DIALOG_STYLE_TABLIST_HEADERS, "SERVER: Buy clothes", string, "Select", "Cancel");				
	}

	if(clickedid == AccsTD[5]) show_accs_dialog(playerid);

	if(clickedid == AccsTD[6]) {
		SetPlayerFacingAngle(playerid, 266.7052);
	}
	if(clickedid == AccsTD[7]) {
		new Float: Angle;
		GetPlayerFacingAngle(playerid, Angle);
		SetPlayerFacingAngle(playerid, Angle-10);
	}
	if(clickedid == AccsTD[8]) {
		new Float: Angle;
		GetPlayerFacingAngle(playerid, Angle);
		SetPlayerFacingAngle(playerid, Angle+10);
	}

	if(clickedid == AccsTD[10]) {
		if(ObjectID[playerid] == 0) return 1;
		SetPVarInt(playerid, "Edited", 0);
		ObjectID[playerid] --;
		update_accs(playerid);
	}
	if(clickedid == AccsTD[11]) {
		if(ObjectID[playerid] == count_objects(ObjectType[playerid])-1) return 1;
		SetPVarInt(playerid, "Edited", 0);
		ObjectID[playerid] ++;
		update_accs(playerid);
	}	

	if(clickedid == AccsTD[12]) EditAttachedObject(playerid, ObjectType[playerid]);

	if(clickedid == Text:INVALID_TEXT_DRAW) {
		if(GetPVarInt(playerid, "accs") == 1) SelectTextDraw(playerid, 0xFFFFFF50);	
	}
	return 1;
}

update_accs(playerid) {
	new 
		string[64],
		i = ObjectSlot[playerid][ObjectID[playerid]];

	if(objectInfo[i][objPremium] == 0) format(string, sizeof(string), "Price: ~g~$%s", FormatNumber(objectInfo[i][objMoney]));
	else if(objectInfo[i][objPremium] > 0 && objectInfo[i][objMoney] == 0) format(string, sizeof(string), "Price: ~b~~h~%s DM", FormatNumber(objectInfo[i][objPremium]));
	else if(objectInfo[i][objPremium] > 0 && objectInfo[i][objMoney] > 0) format(string, sizeof(string), "Price: ~g~$%s~w~~h~ / ~b~~h~%s DM", FormatNumber(objectInfo[i][objMoney]), FormatNumber(objectInfo[i][objPremium]));
	PlayerTextDrawSetString(playerid, AccsPTD[0], string);
	PlayerTextDrawShow(playerid, AccsPTD[0]);

	PlayerTextDrawSetPreviewModel(playerid, AccsPTD[1], objectInfo[i][objModel]);
	PlayerTextDrawShow(playerid, AccsPTD[1]);

	format(string, sizeof(string), "%d/%d", ObjectID[playerid]+1, count_objects(ObjectType[playerid]));
	PlayerTextDrawSetString(playerid, AccsPTD[2], string);
	PlayerTextDrawShow(playerid, AccsPTD[2]);

	SetPlayerAttachedObjectEx( playerid, ObjectType[playerid], objectInfo[i][objModel], objectInfo[i][objBone], objectInfo[i][objfOffsetX], objectInfo[i][objfOffsetY], objectInfo[i][objfOffsetZ], objectInfo[i][objfRotX], objectInfo[i][objfRotY], objectInfo[i][objfRotZ], 1.000000, 1.000000, 1.000000 );
	return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
	switch(dialogid) {
		case DIALOG_ACCS: {
			if(!response) return exit_accs(playerid), attach_objects(playerid);
			attach_objects(playerid);
			select_type(playerid, listitem);
		}
		case DIALOG_BUY_ACCS: {
			if(!response) return 1;
			new id = ObjectSlot[playerid][ObjectID[playerid]];
			if(attachInfo[playerid][aModel][ObjectType[playerid]] != 0) SendClientMessage(playerid, COLOR_GRAD3, "Accesoriul anterior a fost pierdut.");
			switch(listitem) {
				case 0: {
					if(GetPlayerCash(playerid) < objectInfo[id][objMoney]) return SendClientMessage(playerid, COLOR_GREY, "Nu ai suficienti bani!");
					GivePlayerCash(playerid, 0, objectInfo[id][objMoney]);
				}
				case 1: {
					if(PlayerInfo[playerid][pPremiumPoints] < objectInfo[id][objPremium]) return SendClientMessage(playerid, COLOR_GREY, "Nu ai suficienti bani!");
					PlayerInfo[playerid][pPremiumPoints] -= objectInfo[id][objPremium];
					Update(playerid, pPremiumPointsx);	
				}				
			}
			buy_accs(playerid);	
		}
		case DIALOG_EDITACCS: {
			if(!response) return 1;
			if(listitem == 8) return reset_player_accs(playerid), SendClientMessage(playerid, 0xFFADADFF, "Accesorii sterse!");
			if(attachInfo[playerid][aModel][listitem] == 0) return SCM(playerid, COLOR_WARNING, "Eroare: {ffffff}Nu ai un accesoriu pe acest slot!");
			ShowPlayerDialog(playerid, DIALOG_EDITACCS+1, DIALOG_STYLE_MSGBOX, "Edit accesory", "Ce doresti sa modifici la acest accesoriu?", "Pozitia", "Status");
			SetPVarInt(playerid, "listitem", listitem);
			ObjectType[playerid] = listitem;
		}

		case DIALOG_EDITACCS+1: {
			listitem = GetPVarInt(playerid, "listitem");
			if(attachInfo[playerid][aModel][listitem] == 0) return SCM(playerid, COLOR_WARNING, "Eroare: {ffffff}Nu ai un accesoriu pe acest slot!");
			if(response) EditAttachedObject(playerid, listitem);
			else {
				if(attachInfo[playerid][aStatus][listitem] == 0) {
					attachInfo[playerid][aStatus][listitem]	= 1;
					RemovePlayerAttachedObject(playerid, listitem);
					SendClientMessage(playerid, 0xFFADADFF, "Accesoriu dezactivat!");
				}	
				else {
					attachInfo[playerid][aStatus][listitem]	= 0;
					attach_objects(playerid);
					SendClientMessage(playerid, 0xFFADADFF, "Accesoriu activat!");
				}
				save_player_accs(playerid, SAVE_STATUS);	
			}	
		}
	}
	return 1;
}

YCMD:accs(playerid, params[], help) {
	if(!IsAtClothShop(playerid)) return SCM(playerid, COLOR_LGREEN, "Eroare: Nu esti intr-un magazin de haine!");
	SetPVarInt(playerid, "accs", 1);
	show_accs_dialog(playerid);
	SetPlayerPosEx(false, playerid, 217.9727,-98.4919,1005.2578);
	SetPlayerFacingAngle(playerid, 266.7052);
	return 1;
}

exit_accs(playerid) {
	hide_accs_tds(playerid);
	SetPVarInt(playerid, "accs", 0);
	CancelSelectTextDraw(playerid);
	return 1;
}

show_accs_dialog(playerid) {
	new string[128];
	format(string, sizeof(string), "Hats (%d)\nGlasses (%d)\nWatches (%d)\nLasers (%d)\nHand Items (%d)\nMasks & Gang Bandanas (%d)\nBack Items (%d)\nSpecial Items (%d)", 
	count_objects(0), count_objects(1), count_objects(2), count_objects(3), count_objects(4), count_objects(5), count_objects(6), count_objects(7));
	ShowPlayerDialog(playerid, DIALOG_ACCS, DIALOG_STYLE_LIST, "SERVER: Clothes", string, "Select", "Cancel");
}

select_type(playerid, type) {
	new x;
	for(new i = 0; i < MAX_AOBJECTS; i++) {
		if(objectInfo[i][objType] == type && objectInfo[i][objModel]) ObjectSlot[playerid][x] = i, x++;
	}
	ObjectType[playerid] = type;
	ObjectID[playerid] = 0;
	show_accs_tds(playerid);
	update_accs(playerid);
	return 1;
}

YCMD:showme(playerid, params[], help) {
	if(PlayerInfo[playerid][pAdmin] < 6) return 1;
	if(strval(params) == 0) return 1;
	new x = strval(params);
	SetPlayerAttachedObjectEx( playerid, 0, objectInfo[x][objModel], objectInfo[x][objBone], objectInfo[x][objfOffsetX], objectInfo[x][objfOffsetY], objectInfo[x][objfOffsetZ], objectInfo[x][objfRotX], objectInfo[x][objfRotY], objectInfo[x][objfRotZ], 1.000000, 1.000000, 1.000000 );
	return 1;
}

YCMD:insertobject(playerid, params[], help) {
	new model, type;
	if(sscanf(params, "i", model, type)) return SendClientMessage(playerid, -1, "/insertobject <model> <type>");
	new query[256];
	format(query, sizeof(query), "INSERT INTO `objects_accesory` (`Model`) VALUES ('%d')", model, type);
	mysql_tquery(SQL, query, "", "");
	SCM(playerid, COLOR_LOGS, "Object added!");
	return 1;
}

count_objects(type) {
	new x;
	for(new i = 0; i < MAX_AOBJECTS; i++) {
		if(objectInfo[i][objType] == type && objectInfo[i][objModel]) x++;
	}
	return x;
}

check_save(playerid) {
	new 
		query[128],
		Cache: result,
		test;
		
	format(query, sizeof(query), "SELECT * FROM `player_accessory` WHERE `UserID`='%d'", PlayerInfo[playerid][pSQLID]);
	result = mysql_query(SQL, query);
	test = cache_num_rows();
	
    for(new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++) {
    	RemovePlayerAttachedObject(playerid, i);
    }

	if(test == 0) {
		format(query, sizeof(query), "INSERT INTO `player_accessory` (`UserID`) VALUES ('%d')", PlayerInfo[playerid][pSQLID]);
		mysql_tquery(SQL, query, "", "");		
		reset_player_accs(playerid);	
	}
	else {
		new 
			var[80];

		cache_get_value_name(0, "Accessory", query); 
		format(var, sizeof(var), query);
		sscanf(var, "p<|>iiiiiiiiii", attachInfo[playerid][aModel][0], attachInfo[playerid][aModel][1], attachInfo[playerid][aModel][2], attachInfo[playerid][aModel][3], attachInfo[playerid][aModel][4], attachInfo[playerid][aModel][5],
		attachInfo[playerid][aModel][6], attachInfo[playerid][aModel][7], attachInfo[playerid][aModel][8], attachInfo[playerid][aModel][9]);

		cache_get_value_name(0, "Bone", query); 
		format(var, sizeof(var), query);
		sscanf(var, "p<|>iiiiiiiiii", attachInfo[playerid][aBone][0], attachInfo[playerid][aBone][1], attachInfo[playerid][aBone][2], attachInfo[playerid][aBone][3], attachInfo[playerid][aBone][4], attachInfo[playerid][aBone][5],
		attachInfo[playerid][aBone][6], attachInfo[playerid][aBone][7], attachInfo[playerid][aBone][8], attachInfo[playerid][aBone][9]);

		cache_get_value_name(0, "Status", query); 
		format(var, sizeof(var), query);
		sscanf(var, "p<|>iiiiiiiiii", attachInfo[playerid][aStatus][0], attachInfo[playerid][aStatus][1], attachInfo[playerid][aStatus][2], attachInfo[playerid][aStatus][3], attachInfo[playerid][aStatus][4], attachInfo[playerid][aStatus][5],
		attachInfo[playerid][aStatus][6], attachInfo[playerid][aStatus][7], attachInfo[playerid][aStatus][8], attachInfo[playerid][aStatus][9]);

		cache_get_value_name(0, "Index0", query); 
		format(var, sizeof(var), query);
		sscanf(var, "p<|>ffffff", attachInfo[playerid][aIndex0][0], attachInfo[playerid][aIndex0][1], attachInfo[playerid][aIndex0][2], attachInfo[playerid][aIndex0][3], attachInfo[playerid][aIndex0][4], attachInfo[playerid][aIndex0][5]);

		cache_get_value_name(0, "Index1", query); 
		format(var, sizeof(var), query);
		sscanf(var, "p<|>ffffff", attachInfo[playerid][aIndex1][0], attachInfo[playerid][aIndex1][1], attachInfo[playerid][aIndex1][2], attachInfo[playerid][aIndex1][3], attachInfo[playerid][aIndex1][4], attachInfo[playerid][aIndex1][5]);

		cache_get_value_name(0, "Index2", query); 
		format(var, sizeof(var), query);
		sscanf(var, "p<|>ffffff", attachInfo[playerid][aIndex2][0], attachInfo[playerid][aIndex2][1], attachInfo[playerid][aIndex2][2], attachInfo[playerid][aIndex2][3], attachInfo[playerid][aIndex2][4], attachInfo[playerid][aIndex2][5]);

		cache_get_value_name(0, "Index3", query);  
		format(var, sizeof(var), query);
		sscanf(var, "p<|>ffffff", attachInfo[playerid][aIndex3][0], attachInfo[playerid][aIndex3][1], attachInfo[playerid][aIndex3][2], attachInfo[playerid][aIndex3][3], attachInfo[playerid][aIndex3][4], attachInfo[playerid][aIndex3][5]);

		cache_get_value_name(0, "Index4", query); 
		format(var, sizeof(var), query);
		sscanf(var, "p<|>ffffff", attachInfo[playerid][aIndex4][0], attachInfo[playerid][aIndex4][1], attachInfo[playerid][aIndex4][2], attachInfo[playerid][aIndex4][3], attachInfo[playerid][aIndex4][4], attachInfo[playerid][aIndex4][5]);

		cache_get_value_name(0, "Index5", query); 
		format(var, sizeof(var), query);
		sscanf(var, "p<|>ffffff", attachInfo[playerid][aIndex5][0], attachInfo[playerid][aIndex5][1], attachInfo[playerid][aIndex5][2], attachInfo[playerid][aIndex5][3], attachInfo[playerid][aIndex5][4], attachInfo[playerid][aIndex5][5]);

		cache_get_value_name(0, "Index6", query);  
		format(var, sizeof(var), query);
		sscanf(var, "p<|>ffffff", attachInfo[playerid][aIndex6][0], attachInfo[playerid][aIndex6][1], attachInfo[playerid][aIndex6][2], attachInfo[playerid][aIndex6][3], attachInfo[playerid][aIndex6][4], attachInfo[playerid][aIndex6][5]);

		cache_get_value_name(0, "Index7", query); 
		format(var, sizeof(var), query);
		sscanf(var, "p<|>ffffff", attachInfo[playerid][aIndex7][0], attachInfo[playerid][aIndex7][1], attachInfo[playerid][aIndex7][2], attachInfo[playerid][aIndex7][3], attachInfo[playerid][aIndex7][4], attachInfo[playerid][aIndex7][5]);

		cache_get_value_name(0, "Index8", query);  
		format(var, sizeof(var), query);
		sscanf(var, "p<|>ffffff", attachInfo[playerid][aIndex8][0], attachInfo[playerid][aIndex8][1], attachInfo[playerid][aIndex8][2], attachInfo[playerid][aIndex8][3], attachInfo[playerid][aIndex8][4], attachInfo[playerid][aIndex8][5]);

		cache_get_value_name(0, "Index9", query); 
		format(var, sizeof(var), query);
		sscanf(var, "p<|>ffffff", attachInfo[playerid][aIndex9][0], attachInfo[playerid][aIndex9][1], attachInfo[playerid][aIndex9][2], attachInfo[playerid][aIndex9][3], attachInfo[playerid][aIndex9][4], attachInfo[playerid][aIndex9][5]);
		cache_delete(result);	
	}								
	return 1;
}

function load_objects_accesory() {
	mysql_tquery(SQL, "SELECT * FROM `objects_accesory`", "", "");
	return true;
}
function Load_Player_Objects() {
	new x;
	for(new i, j = cache_num_rows(); i != j; i++) {
	    total_objects ++;
       	cache_get_value_name_int(i, 	"ID", objectInfo[x][objID]);
       	objectInfo[x][objID] 		= x;
       	cache_get_value_name_int(i, "Model", objectInfo[x][objID]);
		cache_get_value_name_int(i, "Type", objectInfo[x][objType]);
		cache_get_value_name_int(i, "Money", objectInfo[x][objMoney]);
		cache_get_value_name_int(i, "Premium", objectInfo[x][objPremium]);
  		cache_get_value_name_int(i, "Bone", objectInfo[x][objBone]);
   		cache_get_value_name_float(i, "fOffsetX", objectInfo[x][objfOffsetX]);
   		cache_get_value_name_float(i, "fOffsetY", objectInfo[x][objfOffsetY]);
   		cache_get_value_name_float(i, "fOffsetZ", objectInfo[x][objfOffsetZ]);
   		cache_get_value_name_float(i, "fRotX", objectInfo[x][objfRotX]);
   		cache_get_value_name_float(i, "fRotY", objectInfo[x][objfRotY]);
   		cache_get_value_name_float(i, "fRotZ", objectInfo[x][objfRotZ]);   		  			
	}
	printf("Accesory loaded: %d", total_objects);
	return true;
}

buy_accs(playerid) {
	new 
		string[128],
		type = ObjectType[playerid],
		x = ObjectSlot[playerid][ObjectID[playerid]];

	attachInfo[playerid][aModel][type] = objectInfo[x][objModel];
	attachInfo[playerid][aStatus][type] = 0;
	attachInfo[playerid][aBone][type] = objectInfo[x][objBone];
	if(GetPVarInt(playerid, "Edited") == 0) {
		switch(type) {
			case 0: {
				attachInfo[playerid][aIndex0][0] = objectInfo[x][objfOffsetX]; attachInfo[playerid][aIndex0][1] = objectInfo[x][objfOffsetY]; attachInfo[playerid][aIndex0][2] = objectInfo[x][objfOffsetZ];
				attachInfo[playerid][aIndex0][3] = objectInfo[x][objfRotX]; attachInfo[playerid][aIndex0][4] = objectInfo[x][objfRotY]; attachInfo[playerid][aIndex0][5] = objectInfo[x][objfRotZ];
				save_player_accs(playerid, SAVE_INDEX_0);
			}
			case 1: {
				attachInfo[playerid][aIndex1][0] = objectInfo[x][objfOffsetX]; attachInfo[playerid][aIndex1][1] = objectInfo[x][objfOffsetY]; attachInfo[playerid][aIndex1][2] = objectInfo[x][objfOffsetZ];
				attachInfo[playerid][aIndex1][3] = objectInfo[x][objfRotX]; attachInfo[playerid][aIndex1][4] = objectInfo[x][objfRotY]; attachInfo[playerid][aIndex1][5] = objectInfo[x][objfRotZ];
				save_player_accs(playerid, SAVE_INDEX_1);
			}
			case 2: {
				attachInfo[playerid][aIndex2][0] = objectInfo[x][objfOffsetX]; attachInfo[playerid][aIndex2][1] = objectInfo[x][objfOffsetY]; attachInfo[playerid][aIndex2][2] = objectInfo[x][objfOffsetZ];
				attachInfo[playerid][aIndex2][3] = objectInfo[x][objfRotX]; attachInfo[playerid][aIndex2][4] = objectInfo[x][objfRotY]; attachInfo[playerid][aIndex2][5] = objectInfo[x][objfRotZ];
				save_player_accs(playerid, SAVE_INDEX_2);
			}
			case 3: {
				attachInfo[playerid][aIndex3][0] = objectInfo[x][objfOffsetX]; attachInfo[playerid][aIndex3][1] = objectInfo[x][objfOffsetY]; attachInfo[playerid][aIndex3][2] = objectInfo[x][objfOffsetZ];
				attachInfo[playerid][aIndex3][3] = objectInfo[x][objfRotX]; attachInfo[playerid][aIndex3][4] = objectInfo[x][objfRotY]; attachInfo[playerid][aIndex3][5] = objectInfo[x][objfRotZ];
				save_player_accs(playerid, SAVE_INDEX_3);
			}
			case 4: {
				attachInfo[playerid][aIndex4][0] = objectInfo[x][objfOffsetX]; attachInfo[playerid][aIndex4][1] = objectInfo[x][objfOffsetY]; attachInfo[playerid][aIndex4][2] = objectInfo[x][objfOffsetZ];
				attachInfo[playerid][aIndex4][3] = objectInfo[x][objfRotX]; attachInfo[playerid][aIndex4][4] = objectInfo[x][objfRotY]; attachInfo[playerid][aIndex4][5] = objectInfo[x][objfRotZ];
				save_player_accs(playerid, SAVE_INDEX_4);
			}
			case 5: {
				attachInfo[playerid][aIndex5][0] = objectInfo[x][objfOffsetX]; attachInfo[playerid][aIndex5][1] = objectInfo[x][objfOffsetY]; attachInfo[playerid][aIndex5][2] = objectInfo[x][objfOffsetZ];
				attachInfo[playerid][aIndex5][3] = objectInfo[x][objfRotX]; attachInfo[playerid][aIndex5][4] = objectInfo[x][objfRotY]; attachInfo[playerid][aIndex5][5] = objectInfo[x][objfRotZ];
				save_player_accs(playerid, SAVE_INDEX_5);
			}
			case 6: {
				attachInfo[playerid][aIndex6][0] = objectInfo[x][objfOffsetX]; attachInfo[playerid][aIndex6][1] = objectInfo[x][objfOffsetY]; attachInfo[playerid][aIndex6][2] = objectInfo[x][objfOffsetZ];
				attachInfo[playerid][aIndex6][3] = objectInfo[x][objfRotX]; attachInfo[playerid][aIndex6][4] = objectInfo[x][objfRotY]; attachInfo[playerid][aIndex6][5] = objectInfo[x][objfRotZ];
				save_player_accs(playerid, SAVE_INDEX_6);
			}
			case 7: {
				attachInfo[playerid][aIndex7][0] = objectInfo[x][objfOffsetX]; attachInfo[playerid][aIndex7][1] = objectInfo[x][objfOffsetY]; attachInfo[playerid][aIndex7][2] = objectInfo[x][objfOffsetZ];
				attachInfo[playerid][aIndex7][3] = objectInfo[x][objfRotX]; attachInfo[playerid][aIndex7][4] = objectInfo[x][objfRotY]; attachInfo[playerid][aIndex7][5] = objectInfo[x][objfRotZ];
				save_player_accs(playerid, SAVE_INDEX_7);
			}
			case 8: {
				attachInfo[playerid][aIndex8][0] = objectInfo[x][objfOffsetX]; attachInfo[playerid][aIndex8][1] = objectInfo[x][objfOffsetY]; attachInfo[playerid][aIndex8][2] = objectInfo[x][objfOffsetZ];
				attachInfo[playerid][aIndex8][3] = objectInfo[x][objfRotX]; attachInfo[playerid][aIndex8][4] = objectInfo[x][objfRotY]; attachInfo[playerid][aIndex8][5] = objectInfo[x][objfRotZ];
				save_player_accs(playerid, SAVE_INDEX_8);
			}
			case 9: {
				attachInfo[playerid][aIndex9][0] = objectInfo[x][objfOffsetX]; attachInfo[playerid][aIndex9][1] = objectInfo[x][objfOffsetY]; attachInfo[playerid][aIndex9][2] = objectInfo[x][objfOffsetZ];
				attachInfo[playerid][aIndex9][3] = objectInfo[x][objfRotX]; attachInfo[playerid][aIndex9][4] = objectInfo[x][objfRotY]; attachInfo[playerid][aIndex9][5] = objectInfo[x][objfRotZ];
				save_player_accs(playerid, SAVE_INDEX_9);
			}								
		}
	}

	format(string, sizeof(string), "Ti-ai cumparat accesoriul %d. Toate accesoriile vor fi puse dupa ce vei iesi din meniu.", objectInfo[x][objModel]);
	SendClientMessage(playerid, COLOR_GRAD3, string);

	save_player_accs(playerid, SAVE_ACCESORY);
	save_player_accs(playerid, SAVE_STATUS);		
	save_player_accs(playerid, SAVE_BONE);	
	return 1;
}
attach_objects(playerid) {
    for(new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++) RemovePlayerAttachedObject(playerid, i);
	if(attachInfo[playerid][aStatus][0] == 0 && attachInfo[playerid][aModel][0] != 0) SetPlayerAttachedObjectEx(playerid, 0, attachInfo[playerid][aModel][0], attachInfo[playerid][aBone][0], attachInfo[playerid][aIndex0][0], attachInfo[playerid][aIndex0][1], attachInfo[playerid][aIndex0][2], attachInfo[playerid][aIndex0][3], attachInfo[playerid][aIndex0][4], attachInfo[playerid][aIndex0][5], 1.000000, 1.000000, 1.000000);
	if(attachInfo[playerid][aStatus][1] == 0 && attachInfo[playerid][aModel][1] != 0) SetPlayerAttachedObjectEx(playerid, 1, attachInfo[playerid][aModel][1], attachInfo[playerid][aBone][1], attachInfo[playerid][aIndex1][0], attachInfo[playerid][aIndex1][1], attachInfo[playerid][aIndex1][2], attachInfo[playerid][aIndex1][3], attachInfo[playerid][aIndex1][4], attachInfo[playerid][aIndex1][5], 1.000000, 1.000000, 1.000000);
	if(attachInfo[playerid][aStatus][2] == 0 && attachInfo[playerid][aModel][2] != 0) SetPlayerAttachedObjectEx(playerid, 2, attachInfo[playerid][aModel][2], attachInfo[playerid][aBone][2], attachInfo[playerid][aIndex2][0], attachInfo[playerid][aIndex2][1], attachInfo[playerid][aIndex2][2], attachInfo[playerid][aIndex2][3], attachInfo[playerid][aIndex2][4], attachInfo[playerid][aIndex2][5], 1.000000, 1.000000, 1.000000);
	if(attachInfo[playerid][aStatus][3] == 0 && attachInfo[playerid][aModel][3] != 0) SetPlayerAttachedObjectEx(playerid, 3, attachInfo[playerid][aModel][3], attachInfo[playerid][aBone][3], attachInfo[playerid][aIndex3][0], attachInfo[playerid][aIndex3][1], attachInfo[playerid][aIndex3][2], attachInfo[playerid][aIndex3][3], attachInfo[playerid][aIndex3][4], attachInfo[playerid][aIndex3][5], 1.000000, 1.000000, 1.000000);
	if(attachInfo[playerid][aStatus][4] == 0 && attachInfo[playerid][aModel][4] != 0) SetPlayerAttachedObjectEx(playerid, 4, attachInfo[playerid][aModel][4], attachInfo[playerid][aBone][4], attachInfo[playerid][aIndex4][0], attachInfo[playerid][aIndex4][1], attachInfo[playerid][aIndex4][2], attachInfo[playerid][aIndex4][3], attachInfo[playerid][aIndex4][4], attachInfo[playerid][aIndex4][5], 1.000000, 1.000000, 1.000000);
	if(attachInfo[playerid][aStatus][5] == 0 && attachInfo[playerid][aModel][5] != 0) SetPlayerAttachedObjectEx(playerid, 5, attachInfo[playerid][aModel][5], attachInfo[playerid][aBone][5], attachInfo[playerid][aIndex5][0], attachInfo[playerid][aIndex5][1], attachInfo[playerid][aIndex5][2], attachInfo[playerid][aIndex5][3], attachInfo[playerid][aIndex5][4], attachInfo[playerid][aIndex5][5], 1.000000, 1.000000, 1.000000);
	if(attachInfo[playerid][aStatus][6] == 0 && attachInfo[playerid][aModel][6] != 0) SetPlayerAttachedObjectEx(playerid, 6, attachInfo[playerid][aModel][6], attachInfo[playerid][aBone][6], attachInfo[playerid][aIndex6][0], attachInfo[playerid][aIndex6][1], attachInfo[playerid][aIndex6][2], attachInfo[playerid][aIndex6][3], attachInfo[playerid][aIndex6][4], attachInfo[playerid][aIndex6][5], 1.000000, 1.000000, 1.000000);
	if(attachInfo[playerid][aStatus][7] == 0 && attachInfo[playerid][aModel][7] != 0) SetPlayerAttachedObjectEx(playerid, 7, attachInfo[playerid][aModel][7], attachInfo[playerid][aBone][7], attachInfo[playerid][aIndex7][0], attachInfo[playerid][aIndex7][1], attachInfo[playerid][aIndex7][2], attachInfo[playerid][aIndex7][3], attachInfo[playerid][aIndex7][4], attachInfo[playerid][aIndex7][5], 1.000000, 1.000000, 1.000000);
	if(attachInfo[playerid][aStatus][8] == 0 && attachInfo[playerid][aModel][8] != 0) SetPlayerAttachedObjectEx(playerid, 8, attachInfo[playerid][aModel][8], attachInfo[playerid][aBone][8], attachInfo[playerid][aIndex8][0], attachInfo[playerid][aIndex8][1], attachInfo[playerid][aIndex8][2], attachInfo[playerid][aIndex8][3], attachInfo[playerid][aIndex8][4], attachInfo[playerid][aIndex8][5], 1.000000, 1.000000, 1.000000);	
	if(attachInfo[playerid][aStatus][9] == 0 && attachInfo[playerid][aModel][9] != 0) SetPlayerAttachedObjectEx(playerid, 9, attachInfo[playerid][aModel][9], attachInfo[playerid][aBone][9], attachInfo[playerid][aIndex9][0], attachInfo[playerid][aIndex9][1], attachInfo[playerid][aIndex9][2], attachInfo[playerid][aIndex9][3], attachInfo[playerid][aIndex9][4], attachInfo[playerid][aIndex9][5], 1.000000, 1.000000, 1.000000);
	return 1;
}

YCMD:resettt(playerid, params[], help) {
    for(new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++) {
    	RemovePlayerAttachedObject(playerid, i);
    }
	return 1;
}
reset_player_accs(playerid) {
	for(new i = 0; i < 6; i++) {
		attachInfo[playerid][aIndex0][i] = 0;
		attachInfo[playerid][aIndex1][i] = 0;
		attachInfo[playerid][aIndex2][i] = 0;
		attachInfo[playerid][aIndex3][i] = 0;
		attachInfo[playerid][aIndex4][i] = 0;
		attachInfo[playerid][aIndex5][i] = 0;
		attachInfo[playerid][aIndex6][i] = 0;
		attachInfo[playerid][aIndex7][i] = 0;
		attachInfo[playerid][aIndex8][i] = 0;
		attachInfo[playerid][aIndex9][i] = 0;
	}	
    for(new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++) {
    	RemovePlayerAttachedObject(playerid, i);
    	attachInfo[playerid][aModel][i] = 0, attachInfo[playerid][aBone][i] = 0, attachInfo[playerid][aStatus][i] = 0;
    }
    for(new i = 0; i < 13; i++) save_player_accs(playerid, i);     	
	return 1;
}

save_player_accs(playerid, type) {
	new 
		query[256];

	switch(type) {	
		case SAVE_ACCESORY: {
			mysql_format(SQL, query, sizeof(query), "UPDATE `player_accessory` SET `Accessory` = '%d|%d|%d|%d|%d|%d|%d|%d|%d|%d' WHERE `UserID` = %d", attachInfo[playerid][aModel][0], attachInfo[playerid][aModel][1], attachInfo[playerid][aModel][2], attachInfo[playerid][aModel][3], attachInfo[playerid][aModel][4], attachInfo[playerid][aModel][5],
			attachInfo[playerid][aModel][6], attachInfo[playerid][aModel][7], attachInfo[playerid][aModel][8], attachInfo[playerid][aModel][9], PlayerInfo[playerid][pSQLID]);
		}
		case SAVE_BONE: {
			mysql_format(SQL, query, sizeof(query), "UPDATE `player_accessory` SET `Bone` = '%d|%d|%d|%d|%d|%d|%d|%d|%d|%d' WHERE `UserID` = %d", attachInfo[playerid][aBone][0], attachInfo[playerid][aBone][1], attachInfo[playerid][aBone][2], attachInfo[playerid][aBone][3], attachInfo[playerid][aBone][4], attachInfo[playerid][aBone][5],
			attachInfo[playerid][aBone][6], attachInfo[playerid][aBone][7], attachInfo[playerid][aBone][8], attachInfo[playerid][aBone][9], PlayerInfo[playerid][pSQLID]);
		}		
		case SAVE_STATUS: {
			mysql_format(SQL, query, sizeof(query), "UPDATE `player_accessory` SET `Status` = '%d|%d|%d|%d|%d|%d|%d|%d|%d|%d' WHERE `UserID` = %d", attachInfo[playerid][aStatus][0], attachInfo[playerid][aStatus][1], attachInfo[playerid][aStatus][2], attachInfo[playerid][aStatus][3], attachInfo[playerid][aStatus][4], attachInfo[playerid][aStatus][5],
			attachInfo[playerid][aStatus][6], attachInfo[playerid][aStatus][7], attachInfo[playerid][aStatus][8], attachInfo[playerid][aStatus][9], PlayerInfo[playerid][pSQLID]);	
		}
		case SAVE_INDEX_0: {
			mysql_format(SQL, query, sizeof(query), "UPDATE `player_accessory` SET `Index0` = '%f|%f|%f|%f|%f|%f' WHERE `UserID` = %d", attachInfo[playerid][aIndex0][0], attachInfo[playerid][aIndex0][1], attachInfo[playerid][aIndex0][2], attachInfo[playerid][aIndex0][3], attachInfo[playerid][aIndex0][4], 
			attachInfo[playerid][aIndex0][5], PlayerInfo[playerid][pSQLID]);	
		}
		case SAVE_INDEX_1: {
			mysql_format(SQL, query, sizeof(query), "UPDATE `player_accessory` SET `Index1` = '%f|%f|%f|%f|%f|%f' WHERE `UserID` = %d", attachInfo[playerid][aIndex1][0], attachInfo[playerid][aIndex1][1], attachInfo[playerid][aIndex1][2], attachInfo[playerid][aIndex1][3], attachInfo[playerid][aIndex1][4], 
			attachInfo[playerid][aIndex1][5], PlayerInfo[playerid][pSQLID]);	
		}
		case SAVE_INDEX_2: {
			mysql_format(SQL, query, sizeof(query), "UPDATE `player_accessory` SET `Index2` = '%f|%f|%f|%f|%f|%f' WHERE `UserID` = %d", attachInfo[playerid][aIndex2][0], attachInfo[playerid][aIndex2][1], attachInfo[playerid][aIndex2][2], attachInfo[playerid][aIndex2][3], attachInfo[playerid][aIndex2][4], 
			attachInfo[playerid][aIndex2][5], PlayerInfo[playerid][pSQLID]);	
		}
		case SAVE_INDEX_3: {
			mysql_format(SQL, query, sizeof(query), "UPDATE `player_accessory` SET `Index3` = '%f|%f|%f|%f|%f|%f' WHERE `UserID` = %d", attachInfo[playerid][aIndex3][0], attachInfo[playerid][aIndex3][1], attachInfo[playerid][aIndex3][2], attachInfo[playerid][aIndex3][3], attachInfo[playerid][aIndex3][4], 
			attachInfo[playerid][aIndex3][5], PlayerInfo[playerid][pSQLID]);	
		}
		case SAVE_INDEX_4: {
			mysql_format(SQL, query, sizeof(query), "UPDATE `player_accessory` SET `Index4` = '%f|%f|%f|%f|%f|%f' WHERE `UserID` = %d", attachInfo[playerid][aIndex4][0], attachInfo[playerid][aIndex4][1], attachInfo[playerid][aIndex4][2], attachInfo[playerid][aIndex4][3], attachInfo[playerid][aIndex4][4], 
			attachInfo[playerid][aIndex4][5], PlayerInfo[playerid][pSQLID]);	
		}
		case SAVE_INDEX_5: {
			mysql_format(SQL, query, sizeof(query), "UPDATE `player_accessory` SET `Index5` = '%f|%f|%f|%f|%f|%f' WHERE `UserID` = %d", attachInfo[playerid][aIndex5][0], attachInfo[playerid][aIndex5][1], attachInfo[playerid][aIndex5][2], attachInfo[playerid][aIndex5][3], attachInfo[playerid][aIndex5][4], 
			attachInfo[playerid][aIndex5][5], PlayerInfo[playerid][pSQLID]);	
		}
		case SAVE_INDEX_6: {
			mysql_format(SQL, query, sizeof(query), "UPDATE `player_accessory` SET `Index6` = '%f|%f|%f|%f|%f|%f' WHERE `UserID` = %d", attachInfo[playerid][aIndex6][0], attachInfo[playerid][aIndex6][1], attachInfo[playerid][aIndex6][2], attachInfo[playerid][aIndex6][3], attachInfo[playerid][aIndex6][4], 
			attachInfo[playerid][aIndex6][5], PlayerInfo[playerid][pSQLID]);	
		}
		case SAVE_INDEX_7: {
			mysql_format(SQL, query, sizeof(query), "UPDATE `player_accessory` SET `Index7` = '%f|%f|%f|%f|%f|%f' WHERE `UserID` = %d", attachInfo[playerid][aIndex7][0], attachInfo[playerid][aIndex7][1], attachInfo[playerid][aIndex7][2], attachInfo[playerid][aIndex7][3], attachInfo[playerid][aIndex7][4], 
			attachInfo[playerid][aIndex7][5], PlayerInfo[playerid][pSQLID]);	
		}
		case SAVE_INDEX_8: {
			mysql_format(SQL, query, sizeof(query), "UPDATE `player_accessory` SET `Index8` = '%f|%f|%f|%f|%f|%f' WHERE `UserID` = %d", attachInfo[playerid][aIndex8][0], attachInfo[playerid][aIndex8][1], attachInfo[playerid][aIndex8][2], attachInfo[playerid][aIndex8][3], attachInfo[playerid][aIndex8][4], 
			attachInfo[playerid][aIndex8][5], PlayerInfo[playerid][pSQLID]);	
		}
		case SAVE_INDEX_9: {
			mysql_format(SQL, query, sizeof(query), "UPDATE `player_accessory` SET `Index9` = '%f|%f|%f|%f|%f|%f' WHERE `UserID` = %d", attachInfo[playerid][aIndex9][0], attachInfo[playerid][aIndex9][1], attachInfo[playerid][aIndex9][2], attachInfo[playerid][aIndex9][3], attachInfo[playerid][aIndex9][4], 
			attachInfo[playerid][aIndex9][5], PlayerInfo[playerid][pSQLID]);	
		}
	}
	mysql_tquery(SQL, query, "", "");		
	return 1;
}

show_accs_tds(playerid) {
	for(new i = 0; i < sizeof(AccsTD); i++) TextDrawShowForPlayer(playerid, AccsTD[i]);
	for(new i = 0; i < sizeof(AccsPTD); i++) PlayerTextDrawShow(playerid, AccsPTD[i]);	
	SelectTextDraw(playerid, 0xFFFFFF50);	
	return 1;
}

hide_accs_tds(playerid) {
	for(new i = 0; i < sizeof(AccsTD); i++) TextDrawHideForPlayer(playerid, AccsTD[i]);
	for(new i = 0; i < sizeof(AccsPTD); i++) PlayerTextDrawHide(playerid, AccsPTD[i]);		
	return 1;
}

load_accs_td() {
	AccsTD[0] = TextDrawCreate(38.333320, 165.940734, "box");
	TextDrawLetterSize(AccsTD[0], 0.000000, 18.833332);
	TextDrawTextSize(AccsTD[0], 157.000000, 0.000000);
	TextDrawAlignment(AccsTD[0], 1);
	TextDrawColor(AccsTD[0], -1);
	TextDrawUseBox(AccsTD[0], 1);
	TextDrawBoxColor(AccsTD[0], 141);
	TextDrawSetShadow(AccsTD[0], 0);
	TextDrawSetOutline(AccsTD[0], 0);
	TextDrawBackgroundColor(AccsTD[0], 255);
	TextDrawFont(AccsTD[0], 1);
	TextDrawSetProportional(AccsTD[0], 1);
	TextDrawSetShadow(AccsTD[0], 0);

	AccsTD[1] = TextDrawCreate(38.500057, 338.518524, "box");
	TextDrawLetterSize(AccsTD[1], 0.000000, -0.133337);
	TextDrawTextSize(AccsTD[1], 156.899993, 0.000000);
	TextDrawAlignment(AccsTD[1], 1);
	TextDrawColor(AccsTD[1], -1);
	TextDrawUseBox(AccsTD[1], 1);
	TextDrawBoxColor(AccsTD[1], 882572287);
	TextDrawSetShadow(AccsTD[1], 0);
	TextDrawSetOutline(AccsTD[1], 0);
	TextDrawBackgroundColor(AccsTD[1], 255);
	TextDrawFont(AccsTD[1], 1);
	TextDrawSetProportional(AccsTD[1], 1);
	TextDrawSetShadow(AccsTD[1], 0);

	AccsTD[2] = TextDrawCreate(38.366718, 166.370361, "box");
	TextDrawLetterSize(AccsTD[2], 0.000000, 6.066660);
	TextDrawTextSize(AccsTD[2], 157.000000, 0.000000);
	TextDrawAlignment(AccsTD[2], 1);
	TextDrawColor(AccsTD[2], -1);
	TextDrawUseBox(AccsTD[2], 1);
	TextDrawBoxColor(AccsTD[2], 882572074);
	TextDrawSetShadow(AccsTD[2], 0);
	TextDrawSetOutline(AccsTD[2], 0);
	TextDrawBackgroundColor(AccsTD[2], 255);
	TextDrawFont(AccsTD[2], 1);
	TextDrawSetProportional(AccsTD[2], 1);
	TextDrawSetShadow(AccsTD[2], 0);

	AccsTD[3] = TextDrawCreate(38.366741, 164.296264, "box");
	TextDrawLetterSize(AccsTD[3], 0.000000, -0.066670);
	TextDrawTextSize(AccsTD[3], 157.000000, 0.000000);
	TextDrawAlignment(AccsTD[3], 1);
	TextDrawColor(AccsTD[3], -1);
	TextDrawUseBox(AccsTD[3], 1);
	TextDrawBoxColor(AccsTD[3], 882572287);
	TextDrawSetShadow(AccsTD[3], 0);
	TextDrawSetOutline(AccsTD[3], 0);
	TextDrawBackgroundColor(AccsTD[3], 255);
	TextDrawFont(AccsTD[3], 1);
	TextDrawSetProportional(AccsTD[3], 1);
	TextDrawSetShadow(AccsTD[3], 0);

	AccsTD[4] = TextDrawCreate(71.333236, 260.103607, "BUY");
	TextDrawLetterSize(AccsTD[4], 0.209333, 1.131258);
	TextDrawTextSize(AccsTD[4], 10.000000, 42.000000);
	TextDrawAlignment(AccsTD[4], 2);
	TextDrawColor(AccsTD[4], -1);
	TextDrawUseBox(AccsTD[4], 1);
	TextDrawBoxColor(AccsTD[4], 882572099);
	TextDrawSetShadow(AccsTD[4], 1);
	TextDrawSetOutline(AccsTD[4], 0);
	TextDrawBackgroundColor(AccsTD[4], 28);
	TextDrawFont(AccsTD[4], 2);
	TextDrawSetProportional(AccsTD[4], 1);
	TextDrawSetShadow(AccsTD[4], 1);
	TextDrawSetSelectable(AccsTD[4], true);

	AccsTD[5] = TextDrawCreate(122.599914, 260.103607, "Cancel");
	TextDrawLetterSize(AccsTD[5], 0.209333, 1.131258);
	TextDrawTextSize(AccsTD[5], 10.000000, 42.000000);
	TextDrawAlignment(AccsTD[5], 2);
	TextDrawColor(AccsTD[5], -1);
	TextDrawUseBox(AccsTD[5], 1);
	TextDrawBoxColor(AccsTD[5], 882572099);
	TextDrawSetShadow(AccsTD[5], 1);
	TextDrawSetOutline(AccsTD[5], 0);
	TextDrawBackgroundColor(AccsTD[5], 28);
	TextDrawFont(AccsTD[5], 2);
	TextDrawSetProportional(AccsTD[5], 1);
	TextDrawSetShadow(AccsTD[5], 1);
	TextDrawSetSelectable(AccsTD[5], true);

	AccsTD[6] = TextDrawCreate(74.666702, 306.562927, "RESET");
	TextDrawLetterSize(AccsTD[6], 0.353666, 1.865481);
	TextDrawTextSize(AccsTD[6], 105.000000, 52.000000);
	TextDrawAlignment(AccsTD[6], 1);
	TextDrawColor(AccsTD[6], -1);
	TextDrawSetShadow(AccsTD[6], 1);
	TextDrawSetOutline(AccsTD[6], 0);
	TextDrawBackgroundColor(AccsTD[6], 255);
	TextDrawFont(AccsTD[6], 2);
	TextDrawSetProportional(AccsTD[6], 1);
	TextDrawSetShadow(AccsTD[6], 1);
	TextDrawSetSelectable(AccsTD[6], true);

	AccsTD[7] = TextDrawCreate(55.000022, 307.507324, "<");
	TextDrawLetterSize(AccsTD[7], 0.353666, 1.865481);
	TextDrawTextSize(AccsTD[7], 66.000000, 20.000000);
	TextDrawAlignment(AccsTD[7], 1);
	TextDrawColor(AccsTD[7], -1);
	TextDrawBoxColor(AccsTD[7], 255);
	TextDrawSetShadow(AccsTD[7], 1);
	TextDrawSetOutline(AccsTD[7], 0);
	TextDrawBackgroundColor(AccsTD[7], 255);
	TextDrawFont(AccsTD[7], 2);
	TextDrawSetProportional(AccsTD[7], 1);
	TextDrawSetShadow(AccsTD[7], 1);
	TextDrawSetSelectable(AccsTD[7], true);

	AccsTD[8] = TextDrawCreate(130.666748, 307.392547, ">");
	TextDrawLetterSize(AccsTD[8], 0.353666, 1.865481);
	TextDrawTextSize(AccsTD[8], 140.000000, 20.000000);
	TextDrawAlignment(AccsTD[8], 1);
	TextDrawColor(AccsTD[8], -1);
	TextDrawBoxColor(AccsTD[8], 255);
	TextDrawSetShadow(AccsTD[8], 1);
	TextDrawSetOutline(AccsTD[8], 0);
	TextDrawBackgroundColor(AccsTD[8], 255);
	TextDrawFont(AccsTD[8], 2);
	TextDrawSetProportional(AccsTD[8], 1);
	TextDrawSetShadow(AccsTD[8], 1);
	TextDrawSetSelectable(AccsTD[8], true);

	AccsTD[9] = TextDrawCreate(38.433361, 299.925842, "box");
	TextDrawLetterSize(AccsTD[9], 0.000000, -0.300000);
	TextDrawTextSize(AccsTD[9], 157.000000, 0.000000);
	TextDrawAlignment(AccsTD[9], 1);
	TextDrawColor(AccsTD[9], -1);
	TextDrawUseBox(AccsTD[9], 1);
	TextDrawBoxColor(AccsTD[9], 121);
	TextDrawSetShadow(AccsTD[9], 0);
	TextDrawSetOutline(AccsTD[9], 0);
	TextDrawBackgroundColor(AccsTD[9], 255);
	TextDrawFont(AccsTD[9], 1);
	TextDrawSetProportional(AccsTD[9], 1);
	TextDrawSetShadow(AccsTD[9], 0);

	AccsTD[10] = TextDrawCreate(56.333347, 185.136886, "<");
	TextDrawLetterSize(AccsTD[10], 0.353666, 1.865481);
	TextDrawTextSize(AccsTD[10], 67.000000, 10.000000);
	TextDrawAlignment(AccsTD[10], 1);
	TextDrawColor(AccsTD[10], -1);
	TextDrawBoxColor(AccsTD[10], 255);
	TextDrawSetShadow(AccsTD[10], 1);
	TextDrawSetOutline(AccsTD[10], 0);
	TextDrawBackgroundColor(AccsTD[10], 255);
	TextDrawFont(AccsTD[10], 2);
	TextDrawSetProportional(AccsTD[10], 1);
	TextDrawSetShadow(AccsTD[10], 1);
	TextDrawSetSelectable(AccsTD[10], true);

	AccsTD[11] = TextDrawCreate(127.666679, 184.307250, ">");
	TextDrawLetterSize(AccsTD[11], 0.353666, 1.865481);
	TextDrawTextSize(AccsTD[11], 137.000000, 10.000000);
	TextDrawAlignment(AccsTD[11], 1);
	TextDrawColor(AccsTD[11], -1);
	TextDrawBoxColor(AccsTD[11], 255);
	TextDrawSetShadow(AccsTD[11], 1);
	TextDrawSetOutline(AccsTD[11], 0);
	TextDrawBackgroundColor(AccsTD[11], 255);
	TextDrawFont(AccsTD[11], 2);
	TextDrawSetProportional(AccsTD[11], 1);
	TextDrawSetShadow(AccsTD[11], 1);
	TextDrawSetSelectable(AccsTD[11], true);

	AccsTD[12] = TextDrawCreate(97.933235, 277.525878, "Edit");
	TextDrawLetterSize(AccsTD[12], 0.209333, 1.131258);
	TextDrawTextSize(AccsTD[12], 10.000000, 42.000000);
	TextDrawAlignment(AccsTD[12], 2);
	TextDrawColor(AccsTD[12], -1);
	TextDrawUseBox(AccsTD[12], 1);
	TextDrawBoxColor(AccsTD[12], 882572099);
	TextDrawSetShadow(AccsTD[12], 1);
	TextDrawSetOutline(AccsTD[12], 0);
	TextDrawBackgroundColor(AccsTD[12], 28);
	TextDrawFont(AccsTD[12], 2);
	TextDrawSetProportional(AccsTD[12], 1);
	TextDrawSetShadow(AccsTD[12], 1);
	TextDrawSetSelectable(AccsTD[12], true);
	return 1;
}

load_accs_ptd(playerid) {
	AccsPTD[0] = CreatePlayerTextDraw(playerid, 95.999977, 238.118499, "Price: ~g~$10.000");
	PlayerTextDrawLetterSize(playerid, AccsPTD[0], 0.207332, 1.168593);
	PlayerTextDrawAlignment(playerid, AccsPTD[0], 2);
	PlayerTextDrawColor(playerid, AccsPTD[0], -1);
	PlayerTextDrawSetShadow(playerid, AccsPTD[0], 1);
	PlayerTextDrawSetOutline(playerid, AccsPTD[0], 0);
	PlayerTextDrawBackgroundColor(playerid, AccsPTD[0], 255);
	PlayerTextDrawFont(playerid, AccsPTD[0], 2);
	PlayerTextDrawSetProportional(playerid, AccsPTD[0], 1);
	PlayerTextDrawSetShadow(playerid, AccsPTD[0], 1);

	AccsPTD[1] = CreatePlayerTextDraw(playerid, 50.999996, 147.533325, "");
	PlayerTextDrawLetterSize(playerid, AccsPTD[1], 0.000000, 0.000000);
	PlayerTextDrawTextSize(playerid, AccsPTD[1], 90.000000, 90.000000);
	PlayerTextDrawAlignment(playerid, AccsPTD[1], 1);
	PlayerTextDrawColor(playerid, AccsPTD[1], -1);
	PlayerTextDrawSetShadow(playerid, AccsPTD[1], 0);
	PlayerTextDrawSetOutline(playerid, AccsPTD[1], 0);
	PlayerTextDrawBackgroundColor(playerid, AccsPTD[1], 0);
	PlayerTextDrawFont(playerid, AccsPTD[1], 5);
	PlayerTextDrawSetProportional(playerid, AccsPTD[1], 0);
	PlayerTextDrawSetShadow(playerid, AccsPTD[1], 0);
	PlayerTextDrawSetPreviewModel(playerid, AccsPTD[1], 18951);
	PlayerTextDrawSetPreviewRot(playerid, AccsPTD[1], 163.000000, 253.000000, 181.000000, 1.204378);

	AccsPTD[2] = CreatePlayerTextDraw(playerid, 156.333389, 164.696228, "1/41");
	PlayerTextDrawLetterSize(playerid, AccsPTD[2], 0.202333, 1.326221);
	PlayerTextDrawAlignment(playerid, AccsPTD[2], 3);
	PlayerTextDrawColor(playerid, AccsPTD[2], -1);
	PlayerTextDrawSetShadow(playerid, AccsPTD[2], 1);
	PlayerTextDrawSetOutline(playerid, AccsPTD[2], 0);
	PlayerTextDrawBackgroundColor(playerid, AccsPTD[2], 255);
	PlayerTextDrawFont(playerid, AccsPTD[2], 2);
	PlayerTextDrawSetProportional(playerid, AccsPTD[2], 1);
	PlayerTextDrawSetShadow(playerid, AccsPTD[2], 1);
	return 1;
}