class xMutatorList extends Actor
    DependsOn(xUtil);

var() array<xUtil.MutatorRecord> MutatorList;

simulated function Init(optional bool bLoadClasses)
{
    local xUtil.MutatorRecord tmp;
    local int i, j;

    class'xUtil'.static.GetMutatorList(MutatorList);

    // sort by name
    for (i=0; i<MutatorList.Length-1; i++)
    {
        for (j=i+1; j<MutatorList.Length; j++)
        {
            if (MutatorList[j].FriendlyName < MutatorList[i].FriendlyName)
            {
                tmp = MutatorList[i];
                MutatorList[i] = MutatorList[j];
                MutatorList[j] = tmp;
            }
        }
    }

    if (bLoadClasses)
        LoadClasses();
}

simulated function LoadClasses()
{
    local int i;

    for (i=0; i<MutatorList.Length; i++)
        MutatorList[i].MutClass = class<Mutator>(DynamicLoadObject(MutatorList[i].ClassName,class'Class'));
}

