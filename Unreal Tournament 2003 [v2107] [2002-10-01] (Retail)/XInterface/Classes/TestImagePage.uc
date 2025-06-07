class TestImagePage extends GUIPage;

#exec OBJ LOAD FILE=InterfaceContent.utx

var GUIImage Img;
var GUIComboBox ImgStyle, ImgAlign, ImgSelect;

function MyOnOpen()
{
	Img = GUIImage(Controls[1]);
	ImgStyle = GUIComboBox(Controls[3]);
	ImgAlign = GUIComboBox(Controls[5]);
	ImgSelect = GUIComboBox(Controls[7]);

	// Prepare the ComboBoxes
	ImgStyle.AddItem("Normal");
	ImgStyle.AddItem("Stretched");
	ImgStyle.AddItem("Scaled");
	ImgStyle.AddItem("Bound");
	ImgStyle.AddItem("Justified");

	ImgAlign.AddItem("TopLeft");
	ImgAlign.AddItem("Center");
	ImgAlign.AddItem("BottomRight");

	ImgSelect.AddItem("PlayerPictures.cEgyptFemaleBA");
	ImgSelect.AddItem("InterfaceContent.Menu.bg07");
	ImgSelect.AddItem("PlayerPictures.Galactic");
	ImgSelect.AddItem("InterfaceContent.Menu.CO_Final");
	ImgSelect.AddItem("InterfaceContent.BorderBoxF_Pulse");

	SetNewImage("PlayerPictures.cEgyptFemaleBA");
}

function OnNewImgStyle(GUIComponent Sender)
{
local string NewImgStyle;

	NewImgStyle = ImgStyle.Get();
	if (NewImgStyle == "Normal")
		Img.ImageStyle=ISTY_Normal;
	else if (NewImgStyle == "Stretched")
		Img.ImageStyle=ISTY_Stretched;
	else if (NewImgStyle == "Scaled")
		Img.ImageStyle=ISTY_Scaled;
	else if (NewImgStyle == "Bound")
		Img.ImageStyle=ISTY_Bound;
	else if (NewImgStyle == "Justified")
		Img.ImageStyle=ISTY_Justified;
}

function OnNewImgAlign(GUIComponent Sender)
{
local string NewImgAlign;

	NewImgAlign = ImgAlign.Get();
	if (NewImgAlign == "TopLeft")
		Img.ImageAlign = IMGA_TopLeft;
	else if (NewImgAlign == "Center")
		Img.ImageAlign = IMGA_Center;
	else if (NewImgAlign == "BottomRight")
		Img.ImageAlign = IMGA_BottomRight;
}

function OnNewImgSelect(GUIComponent Sender)
{
	SetNewImage(ImgSelect.Get());
}

function SetNewImage(string ImageName)
{
	Img.Image=DLOTexture(ImageName);
}

function Material DLOTexture(string TextureFullName)
{
	return Material(DynamicLoadObject(TextureFullName, class'Material'));
}

defaultproperties
{
	Begin Object Class=GUIImage Name=Backdrop
		Image=Material'InterfaceContent.Menu.EditBox'
		WinTop=0.2
		WinLeft=0.1
		WinHeight=0.4
		WinWidth=0.4
		ImageStyle=ISTY_Stretched
	End Object

	Begin Object Class=GUIImage Name=TheImage
		WinTop=0.2
		WinLeft=0.1
		WinHeight=0.4
		WinWidth=0.4
	End Object

	Begin Object Class=GUILabel Name=lblImgStyle
		Caption="Image Style"
		WinTop=0.2
		WinLeft=0.5
		WinWidth=0.2
		WinHeight=0.06
	End Object

	Begin Object Class=GUIComboBox Name=cboImgStyle
		WinTop=0.2
		WinLeft=0.75
		WinHeight=0.06
		WinWidth=0.2
		bReadOnly=true
		OnChange=OnNewImgStyle
	End Object

	Begin Object Class=GUILabel Name=lblImgAlign
		Caption="Image Alignment"
		WinTop=0.3
		WinLeft=0.5
		WinWidth=0.2
		WinHeight=0.06
	End Object

	Begin Object Class=GUIComboBox Name=cboImgAlign
		WinTop=0.3
		WinLeft=0.75
		WinHeight=0.06
		WinWidth=0.2
		bReadOnly=true
		OnChange=OnNewImgAlign
	End Object

	Begin Object Class=GUILabel Name=lblImgSelect
		Caption="Select Image"
		WinTop=0.4
		WinLeft=0.5
		WinWidth=0.2
		WinHeight=0.06
	End Object

	Begin Object Class=GUIComboBox Name=cboImgSelect
		WinTop=0.4
		WinLeft=0.75
		WinHeight=0.06
		WinWidth=0.2
		bReadOnly=true
		OnChange=OnNewImgSelect
	End Object

	Controls(0)=GUIImage'Backdrop'
	Controls(1)=GUIImage'TheImage'
	Controls(2)=GUILabel'lblImgStyle'
	Controls(3)=GUIComboBox'cboImgStyle'
	Controls(4)=GUILabel'lblImgAlign'
	Controls(5)=GUIComboBox'cboImgAlign'
	Controls(6)=GUILabel'lblImgSelect'
	Controls(7)=GUIComboBox'cboImgSelect'

	OnOpen=MyOnOpen
}