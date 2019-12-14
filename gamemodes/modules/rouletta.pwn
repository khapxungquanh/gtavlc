#define DIALOG_BETRED 				1000
#define DIALOG_BETGREEN				1001
#define DIALOG_BETBLACK				1002

new 
	TotalBetPlace[3], PrizePlace[3], PlaceBet[MAX_PLAYERS][3], PlaceAlerdy[MAX_PLAYERS], RulettStatus = 0, Text: RoulettaTD[19], Iterator:IsRuletta<MAX_PLAYERS>, RulettaTime[3], RuletaType[3]
;

function SendRulettaMessage(color, amount[], bool: type) {
	new i;
	if(!type) foreach(i : IsRuletta) SCM(i, color, amount);
	else foreach(i : IsRuletta) PlayAudioStreamForPlayer(i, amount);
	return true;
}
function StartRulettaIn() {
	RulettStatus = 1;
	RulettaTime[0] = 30;
	RulettaTime[2] = RandomEx(70, 150);
	new x = RulettaTime[2], y = RulettaTime[1];
	while(RulettaTime[2]) {
		y = !y ? 36 : y - 1;	
		x--;
		if(x == 0) break;
	}
	RulettaTime[2] = random(10)%2 ? y%2 == 0 ? RulettaTime[2]+1 : RulettaTime[2] : y%2 == 0 ? RulettaTime[2] : RulettaTime[2]+1;
	switch(random(3)) {
		case 0: {
			RuletaType[0] = RulettaTime[2]/8;
			RuletaType[1] = RulettaTime[2]/15;
			RuletaType[2] = RulettaTime[2]/17;
		}
		case 1: {
			RuletaType[0] = RulettaTime[2]/7;
			RuletaType[1] = RulettaTime[2]/13;
			RuletaType[2] = RulettaTime[2]/18;			
		}
		case 2: {
			RuletaType[0] = RulettaTime[2]/10;
			RuletaType[1] = RulettaTime[2]/18;
			RuletaType[2] = RulettaTime[2]/20;			
		}
		case 3: {
			RuletaType[0] = RulettaTime[2]/13;
			RuletaType[1] = RulettaTime[2]/15;
			RuletaType[2] = RulettaTime[2]/16;			
		}
	}
	return true;
}

function MainRuletta() {
	if(RulettaTime[0] == 0) {
		RulettStatus = 2;
		SendRulettaMessage(-1, "http://play.bugged.ro/spinning.mp3", true);
		TextDrawSetString(RoulettaTD[15], "THE ROULETTE IS SPINNING...");
		return true;
	}
	gString[0] = EOS;
	format(gString, 50, "SPINNING IN %02d SECONDS...", RulettaTime[0]);
	TextDrawSetString(RoulettaTD[15], gString);
	RulettaTime[0] --;    
	return true;
}

function UpdateRuletta() {
	gString[0] = EOS;
	RulettaTime[1] = !RulettaTime[1] ? 36 : RulettaTime[1] - 1;
	new bool: t = RulettaTime[1]%2 ? false : true;
	switch(RulettaTime[1]) {
		case 0: TextDrawBoxColor(RoulettaTD[0], 16711935), TextDrawBoxColor(RoulettaTD[1], -16776961), TextDrawBoxColor(RoulettaTD[2], 255), TextDrawBoxColor(RoulettaTD[3], -16776961), TextDrawBoxColor(RoulettaTD[4], 255), TextDrawBoxColor(RoulettaTD[5], -16776961), TextDrawBoxColor(RoulettaTD[6], 255);
		case 1: TextDrawBoxColor(RoulettaTD[0], 255), TextDrawBoxColor(RoulettaTD[1], 16711935), TextDrawBoxColor(RoulettaTD[2], -16776961), TextDrawBoxColor(RoulettaTD[3], 255), TextDrawBoxColor(RoulettaTD[4], -16776961), TextDrawBoxColor(RoulettaTD[5], 255), TextDrawBoxColor(RoulettaTD[6], -16776961);
		case 2: TextDrawBoxColor(RoulettaTD[0], -16776961), TextDrawBoxColor(RoulettaTD[1], 255), TextDrawBoxColor(RoulettaTD[2], 16711935), TextDrawBoxColor(RoulettaTD[3], -16776961), TextDrawBoxColor(RoulettaTD[4], 255), TextDrawBoxColor(RoulettaTD[5], -16776961), TextDrawBoxColor(RoulettaTD[6], 255);
		case 3: TextDrawBoxColor(RoulettaTD[0], 255), TextDrawBoxColor(RoulettaTD[1], -16776961), TextDrawBoxColor(RoulettaTD[2], 255), TextDrawBoxColor(RoulettaTD[3], 16711935), TextDrawBoxColor(RoulettaTD[4], -16776961), TextDrawBoxColor(RoulettaTD[5], 255), TextDrawBoxColor(RoulettaTD[6], -16776961);
		case 4: TextDrawBoxColor(RoulettaTD[0], -16776961), TextDrawBoxColor(RoulettaTD[1], 255), TextDrawBoxColor(RoulettaTD[2], -16776961), TextDrawBoxColor(RoulettaTD[3], 255), TextDrawBoxColor(RoulettaTD[4], 16711935), TextDrawBoxColor(RoulettaTD[5], -16776961), TextDrawBoxColor(RoulettaTD[6], 255);
		case 5: TextDrawBoxColor(RoulettaTD[0], 255), TextDrawBoxColor(RoulettaTD[1], -16776961), TextDrawBoxColor(RoulettaTD[2], 255), TextDrawBoxColor(RoulettaTD[3], -16776961), TextDrawBoxColor(RoulettaTD[4], 255), TextDrawBoxColor(RoulettaTD[5], 16711935), TextDrawBoxColor(RoulettaTD[6], -16776961);
		case 6: TextDrawBoxColor(RoulettaTD[0], -16776961), TextDrawBoxColor(RoulettaTD[1], 255), TextDrawBoxColor(RoulettaTD[2], -16776961), TextDrawBoxColor(RoulettaTD[3], 255), TextDrawBoxColor(RoulettaTD[4], -16776961), TextDrawBoxColor(RoulettaTD[5], 255), TextDrawBoxColor(RoulettaTD[6], 16711935);
		default: TextDrawBoxColor(RoulettaTD[0], t ? (-16776961) : (255)), TextDrawBoxColor(RoulettaTD[1], t ? (255) : (-16776961)), TextDrawBoxColor(RoulettaTD[2], t ? (-16776961) : (255)), TextDrawBoxColor(RoulettaTD[3], t ? (255) : (-16776961)), TextDrawBoxColor(RoulettaTD[4], t ? (-16776961) : (255)), TextDrawBoxColor(RoulettaTD[5], t ? (255) : (-16776961)), TextDrawBoxColor(RoulettaTD[6], t ? (-16776961) : (255));
	}
	foreach(new i : IsRuletta) for(new x = 0; x < 7; x++) TextDrawShowForPlayer(i, RoulettaTD[x]);
	RulettaTime[2] = !RulettaTime[2] ? 0 : RulettaTime[2] - 1;
	if(RulettaTime[2] == 0) {
		RulettStatus = 0;
		SetTimerEx("GiveRouletteParize", 1000, false, "i", t);
	}
	if(RulettaTime[2] == RuletaType[1]) RulettStatus = 3;
	else if(RulettaTime[2] == RuletaType[2]) RulettStatus = 4;
	return true;
}

function GiveRouletteParize(t) {
	new x, color[10];
	switch(RulettaTime[1]) {
		case 1, 4, 6: x = 2, color = "BLACK";
		case 0, 2, 5: x = 0, color = "RED";
		case 3: x = 1, color = "GREEN";
		default: x = t ? (2) : (0), color = t ? ("BLACK") : ("RED");
	}
	format(gString, 50, "The roulette landed on %s", color);
	SendRulettaMessage(COLOR_SERVER, gString, false);
	SendRulettaMessage(-1, "http://play.bugged.ro/tone.mp3", true);
	TextDrawSetString(RoulettaTD[15], "WAITING FOR BETS...");
	foreach(new i : IsRuletta) {
		if(!PlaceAlerdy[i])
			continue;

		if(x != PlaceAlerdy[i]-1) {
			format(gString, 50, "You lost $%s.", FormatNumber(PlaceBet[i][PlaceAlerdy[i]-1]));
			SCM(i, COLOR_MONEY, gString);
		} else {
			format(gString, 50, "You won $%s.", FormatNumber(x != 1 ? PlaceBet[i][x] * 2 : PlaceBet[i][x] * 14));
			SCM(i, COLOR_MONEY, gString);		
			GivePlayerCash(i, 1, x != 1 ? PlaceBet[i][x] * 2 : PlaceBet[i][x] * 14);
		}
		PlaceBet[i][x] = 0;
		PlaceAlerdy[i] = 0;
	}
	for(new c = 0; c < 3; c++) TotalBetPlace[c] = 0, PrizePlace[c] = 0, TextDrawSetString(RoulettaTD[c+16], "0 BETS~n~$0");
	return true;
}