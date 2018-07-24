using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Block : MonoBehaviour {
	// each created Quad has Collider property that allows to attach C# script to it
	public event System.Action<Block>	OnBlockPressed;
	public event System.Action			OnFinishedMoving;

	// we are using own coordinates system, to ignore world own
	public Vector2Int					coord;
	Vector2Int							startingCoord;

	public void		Init(Vector2Int startingCoord, Texture2D image)
	{
		this.startingCoord = startingCoord;
		coord = startingCoord;
		// get rid of all lights for the scene
		// Unity wouldn't find Unlit/Texture by itself, that is why we explicitly gives it shader in Resources folder
		GetComponent<MeshRenderer> ().material.shader = Resources.Load<Material>("Block");
		GetComponent<MeshRenderer> ().material.mainTexture = image;
	}

	public void		MoveToPosition(Vector2 target, float duration)
	{
		StartCoroutine (AnimateMove(target, duration));
	}

	// gets called automatically if collider is attached
	void	OnMouseDown()
	{
		if (OnBlockPressed != null)
			OnBlockPressed(this);
	}

	// block target position, and duration of its smooth movement
	IEnumerator		AnimateMove(Vector2 target, float duration)
	{
		Vector2 initialPos = transform.position;
		float	percent = 0;

		while (percent < 1f)
		{
			percent += Time.deltaTime / duration;
			transform.position = Vector2.Lerp (initialPos, target, percent);
			yield return null;
		}

		if (OnFinishedMoving != null)
			OnFinishedMoving ();
	}

	public	bool	IsAtStartingCoord()
	{
		return coord == startingCoord;
	}
}
