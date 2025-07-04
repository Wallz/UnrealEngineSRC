class xVoicePack extends TeamVoicePack;

static function byte GetMessageIndex(name PhraseName)
{
	local float r;
	r = FRand();

	if ( PhraseName == 'GOTENEMYFLAG' )
		return 2;
	if ( PhraseName == 'INJURED' )
	{
		if ( r < 0.3 )
			return 6;
		else if ( r < 0.5 )
			return 13;
		else
			return 4;
	}
	else if ( PhraseName == 'GOTOURFLAG' )
		return 8;
	else if ( PhraseName == 'NEEDBACKUP' )
	{
		if(r<0.5)
			return 13;
		else
			return 22;
	}
	else if ( PhraseName == 'NEEDOURFLAG' )
		return 1;
	else if ( PhraseName == 'ENEMYFLAGCARRIERHERE' )
		return 12;
	else if ( PhraseName == 'ENEMYBALLCARRIERHERE' )
		return 15;
	else if ( PhraseName == 'INCOMING' )
	{
		if(r < 0.33)
			return 20;
		else if(r < 0.66)
			return 21;
		else
			return 14;
	}
	else if ( PhraseName == 'GOTYOURBACK' )
		return 3;
	else if ( PhraseName == 'MANDOWN' )
		return 5;
	else if ( PhraseName == 'INPOSITION' )
		return 9;
	else if ( PhraseName == 'ONMYWAY' )
		return 10;
}

DefaultProperties
{
	numAcks=4
	AckString(0)="Affirmative"
	AckString(1)="Got It"
	AckString(2)="I'm On It"
	AckString(3)="Roger"

	numFFires=3
	FFireString(0)="I'm On Your Team!"
	FFireString(1)="I'm On Your Team, Idiot!"
	FFireAbbrev(1)="Your Team, Idiot!"
	FFireString(2)="Same Team!"

	TauntString(0)="And Stay Down"
	TauntString(1)="Anyone Else Want Some?"
	TauntString(2)="Boom!"
	TauntString(3)="BURN Baby"
	TauntString(4)="Die Bitch"
	TauntString(5)="Eat THAT"
	TauntString(6)="You Fight Like Nali"
	TauntString(7)="Is That Your Best?"
	TauntString(8)="Kiss My Ass"
	TauntString(9)="Loser"
	TauntString(10)="MY House"
	TauntString(11)="Next!"
	TauntString(12)="Oh YEAH!"
	TauntString(13)="Ownage"
	TauntString(14)="Seeya"
	TauntString(15)="That HAD To Hurt"
	TauntString(16)="Useless"
	TauntString(17)="You Play Like A Girl"
	TauntString(18)="You Be Dead"
	TauntString(19)="You Like That?"
	TauntString(20)="You Whore"

	MatureTaunt(4)=1
	MatureTaunt(8)=1
	MatureTaunt(20)=1

	HumanOnlyTaunt(6)=1
	HumanOnlyTaunt(17)=1

	OrderString(0)="Defend the base"
	OrderAbbrev(0)="Defend"
	OrderString(1)="Hold this position"
	OrderString(2)="Assault the base"
	OrderAbbrev(2)="Attack"
	OrderString(3)="Cover me"
	OrderString(4)="Search and destroy"
	OrderString(10)="Take their flag"
	OrderString(11)="Defend the flag"

	OtherString(0)="Base is undefended!"
	OtherString(1)="Somebody get our flag back!"
	OtherAbbrev(1)="Get our flag!"
	OtherString(2)="I've got the flag"
	OtherAbbrev(2)="Got the flag"
	OtherString(3)="I've got your back"
	OtherAbbrev(3)="Got your back"
	OtherString(4)="I'm hit!"
	OtherString(5)="Man down!"
	OtherString(6)="I'm all alone here"
	OtherAbbrev(6)="All alone!"
	OtherString(7)="Negative!"
	OtherString(8)="I've got our flag"
	OtherAbbrev(8)="Got our flag"
	OtherString(9)="I'm in position %l"
	DisplayOtherMessage(9)=1
	OtherAbbrev(9)="In position"
	OtherString(10)="I'm going in!"
	OtherDelayed(10)=1
	OtherString(11)="Area is secure"
	OtherString(12)="Enemy flag carrier is %l"
	OtherAbbrev(12)="Enemy carrier here"
	DisplayOtherMessage(12)=1
	OtherString(13)="I need some backup"
	OtherString(14)="Incoming!"
	OtherString(15)="Enemy ball carrier is %l"
	OtherAbbrev(15)="Enemy carrier here"
	DisplayOtherMessage(15)=1
	OtherString(16)="Alpha secure!"
	OtherString(17)="Bravo secure!"
	OtherString(18)="Attack Alpha"
	OtherString(19)="Attack Bravo"
	OtherString(20)="The base is under attack %l"
	DisplayOtherMessage(20)=1
	OtherString(21)="We're being overrun %l!"
	DisplayOtherMessage(21)=1
	OtherString(22)="Under heavy attack %l"
	DisplayOtherMessage(22)=1
	OtherString(23)="Defend point Alpha"
	OtherString(24)="Defend point Bravo"
	OtherString(25)="Get The Ball"
	OtherString(26)="I'm on defense"
	OtherString(27)="I'm on offense"
	OtherString(28)="Take point Alpha"
	OtherString(29)="Take point Bravo"
	OtherString(30)="Medic"
	OtherString(31)="Nice"

	OtherMesgGroup(0)="CTFGame"
	OtherMesgGroup(1)="CTFGame"
	OtherMesgGroup(2)="CTFGame"
	OtherMesgGroup(8)="CTFGame"
	OtherMesgGroup(12)="CTFGame"
	OtherMesgGroup(15)="BombingRun"
	OtherMesgGroup(16)="DoubleDom"
	OtherMesgGroup(17)="DoubleDom"
	OtherMesgGroup(18)="DoubleDom"
	OtherMesgGroup(19)="DoubleDom"
	OtherMesgGroup(20)="CTFGame"
	OtherMesgGroup(23)="DoubleDom"
	OtherMesgGroup(24)="DoubleDom"
	OtherMesgGroup(25)="BombingRun"
	OtherMesgGroup(28)="DoubleDom"
	OtherMesgGroup(29)="DoubleDom"
}