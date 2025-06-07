class DrawOpImage extends DrawOpBase;

var Material		Image;
var bool			bStretch;					// Stretch the picture;

function Draw(Canvas Canvas)
{
	Super.Draw(Canvas);

	if (bStretch)
		Canvas.DrawTileStretched(Image, Width * Canvas.SizeX, Height * Canvas.SizeY);
	else
		Canvas.DrawTile(Image, Width * Canvas.SizeX, Height * Canvas.SizeY, 0.0, 0.0, Image.MaterialUSize(), Image.MaterialVSize());

	//Log("DrawOpImage.Draw Called");
}
