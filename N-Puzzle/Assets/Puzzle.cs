using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Puzzle : MonoBehaviour {

	public const int 	blocksPerLine = 4;
	Block				emptyBlock;

	void		Start()
	{
		CreatePuzzle ();
	}

	void		CreatePuzzle()
	{
		for (int y = 0; y < blocksPerLine; y++) {
			for (int x = 0; x < blocksPerLine; x++) {
				// creating single block in the grid
				GameObject blockObject = GameObject.CreatePrimitive(PrimitiveType.Quad);
				// setting position of block on the grid
				blockObject.transform.position = -Vector2.one * (blocksPerLine - 1) * .5f  + new Vector2(x, y);
				blockObject.transform.parent = transform;

				// adding block script to each of created blockObject-s
				Block block = blockObject.AddComponent<Block>();
				block.OnBlockPressed += PlayerMoveBlockInput;

				// hiding block in top right corner
				if (y == 0 && x == blocksPerLine - 1) {
					blockObject.SetActive (false);
					emptyBlock = block;
				}
			}
		}

		// changing the size of the camera, to fit grid nearly completely
		Camera.main.orthographicSize = blocksPerLine * 0.55f;
	}

	void		PlayerMoveBlockInput(Block blockToMove)
	{
		// swapping blocks when pressed
		Vector2		targetPosition = emptyBlock.transform.position;
		emptyBlock.transform.position = blockToMove.transform.position;
		blockToMove.transform.position = targetPosition;
	}
}
