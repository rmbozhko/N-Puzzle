using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class ImageSlicer {

	// 
	public static Texture2D[,]		GetSlices(Texture2D image, int blocksPerLine)
	{
		// if passed image isn't square one
		int		imageSize = Mathf.Min(image.width, image.height);
		// figuring out the size in pixels for each block
		int		blockSize = imageSize / blocksPerLine;

		Texture2D[,] blocks = new Texture2D[blocksPerLine, blocksPerLine];

		for (int y = 0; y < blocksPerLine; y++) {
			for (int x = 0; x < blocksPerLine; x++) {
				Texture2D	block = new Texture2D (blockSize, blockSize);
				// get rid of lines across the image
				block.wrapMode = TextureWrapMode.Clamp;
				// we are multiplying to get correct coordinate in pixels
				block.SetPixels (image.GetPixels(x * blockSize, y * blockSize, blockSize, blockSize));
				block.Apply ();
				blocks [x, y] = block;
			}
		}
		return (blocks);
	}
}
