using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

/* Setting project:
 * 0. Create GameObject, top left window
 * 1. Assign Puzzle.cs to GameObject
 * 2. Move image to files section
 * 3. Set its read/write properties to true, top right menu bar
 */
public class Puzzle : MonoBehaviour {
	public 	int 			blocksPerLine;
	public 	const 	int		shuffleLength = 20;
	public	const	float	defaultMoveDuration = .2f;
	public	const	float	shuffleMoveDuration = .1f;
	public 	Texture2D		image;

	enum PuzzleState
	{
		Solved,
		Shuffling,
		InPlay
	};

	PuzzleState 		state;
	Block				emptyBlock;
	Queue<Block> 		inputs;
	bool				blockIsMoving;
	int					shuffleMovesRemaining;
	Block[,]			blocks;
	Vector2Int			prevShuffleOffset;
	List<String> 		args;

	string 		GetArg(string name, string defaultValue)
    {
        int index = args.IndexOf(name);
        if (index >= 0 && index < args.Count - 1)
        {
            return args[index + 1];
        }
        return defaultValue;
    }

	void		SetImage(string url)
	{
		if (url != null)
		{
			WWW www = new WWW(url);
			if (www.isDone && !string.IsNullOrEmpty(www.error))
	               Debug.Log(www.error);
			image = www.texture;
		}
		else
			Debug.Log("No value found for -img flag");
	}

	void		Start()
	{
		args = System.Environment.GetCommandLineArgs().ToList();
		Debug.Log(System.Environment.GetCommandLineArgs());
		SetImage("https://cdn.intra.42.fr/users/rbozhko.jpg");//GetArg("-img", null));
		blocksPerLine = 4;//Convert.ToInt16(GetArg("-size", null));
		if (blocksPerLine > 0)
			CreatePuzzle ();
		else
			Debug.Log("Puzzle size cannot be negative");
	}

	void		Update()
	{
		// stop shuffling when space is pressed
		if (state == PuzzleState.Solved && Input.GetKeyDown (KeyCode.Space)) {
			StartShuffle ();
		}
	}

	void		CreatePuzzle()
	{
		// initialising 'blocks' used for shuffling
		blocks = new Block[blocksPerLine, blocksPerLine];
		// getting content of the image, splitted into Texture2D bidarr array
		Texture2D[,]	imageSlices = ImageSlicer.GetSlices (image, blocksPerLine);
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
				block.OnFinishedMoving += OnBlockFinishedMoving;
				// we are using own coordinates system, to ignore world own
				block.Init (new Vector2Int(x, y), imageSlices[x, y]);
				blocks [x, y] = block;

				// setting emptyBlock 
				if (y == 0 && x == blocksPerLine - 1)
					emptyBlock = block;
			}
		}

		// changing the size of the camera, to fit grid nearly completely
		Camera.main.orthographicSize = blocksPerLine * 0.55f;
		inputs = new Queue<Block> ();
	}

	void		PlayerMoveBlockInput(Block blockToMove)
	{
		if (state == PuzzleState.InPlay) {
			inputs.Enqueue (blockToMove);
			MakeNextPlayerMove ();
		}
	}

	void		MakeNextPlayerMove()
	{
		while (inputs.Count > 0 && !blockIsMoving)
			MoveBlock (inputs.Dequeue (), defaultMoveDuration);
	}

	void		MoveBlock(Block blockToMove, float duration)
	{
		// swapping blocks when pressed, and blockToMove is near emptyBlock(on the left or right, up or down)
		if ((blockToMove.coord - emptyBlock.coord).sqrMagnitude == 1)
		{
			blocks [blockToMove.coord.x, blockToMove.coord.y] = emptyBlock;
			blocks [emptyBlock.coord.x, emptyBlock.coord.y] = blockToMove;

			Vector2Int	targetCoord = emptyBlock.coord;
			emptyBlock.coord = blockToMove.coord;
			blockToMove.coord = targetCoord;

			Vector2	targetPosition = emptyBlock.transform.position;
			emptyBlock.transform.position = blockToMove.transform.position;
			blockToMove.MoveToPosition(targetPosition, duration);
			blockIsMoving = true;
		}
	}

	void		OnBlockFinishedMoving()
	{
		blockIsMoving = false;
		CheckIfSolved ();

		// we are allowed to swap blocks, only when state is InPlay
		if (state == PuzzleState.InPlay)
			MakeNextPlayerMove ();
		else if (state == PuzzleState.Shuffling)
		{
			if (shuffleMovesRemaining > 0)
				MakeNextShuffleMove ();
			else
				state = PuzzleState.InPlay;
		}
	}

	void		StartShuffle()
	{
		// setting state to shuffling to protect puzzle from mouse input, while it's shuffling
		state = PuzzleState.Shuffling;
		shuffleMovesRemaining = shuffleLength;
		// hiding block emptyBlock
		emptyBlock.gameObject.SetActive (false);
		MakeNextShuffleMove ();
	}

	void		MakeNextShuffleMove()
	{
		// setting blocks-neighbours
		Vector2Int[] offsets = { new Vector2Int(1, 0), new Vector2Int(-1, 0), new Vector2Int(0, 1), new Vector2Int(0, -1)};
		// selecting random neighbour
		int		randIndex = UnityEngine.Random.Range(0, offsets.Length);
		for (int i = 0; i < offsets.Length; i++) {
			// if we won't succeed with randIndex, then we sum it with 'i', and apply modul in order to stay in bounds
			Vector2Int	offset = offsets [(randIndex + i) % offsets.Length];
			// we imply this condition to erase diggling back-and-forth by blocks
			// if block is moved, it would definitely have at least one coord negative
			if (offset != prevShuffleOffset * -1) {
				Vector2Int	moveBlockCoord = emptyBlock.coord + offset;

				// checking whether we can access selected neighbour
				// for instance if we want to swap top right block with its neighbour on the right side

				if ((moveBlockCoord.x >= 0 && moveBlockCoord.x < blocksPerLine) && (moveBlockCoord.y >= 0 && moveBlockCoord.y < blocksPerLine)) {
					MoveBlock (blocks [moveBlockCoord.x, moveBlockCoord.y], shuffleMoveDuration);
					shuffleMovesRemaining--;
					prevShuffleOffset = offset;
					break;
				}
			}
		}
	}

	void		CheckIfSolved()
	{
		foreach (Block block in blocks) {
			if (!block.IsAtStartingCoord())
				return ;
		}
		// if player solves image, then the empty block appears with pixels of image on it
		state = PuzzleState.Solved;
		emptyBlock.gameObject.SetActive (true);
	}
}
