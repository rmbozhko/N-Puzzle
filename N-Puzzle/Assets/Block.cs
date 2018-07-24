using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Block : MonoBehaviour {

	// each created Quad has Collider property that allows to attach C# script to it
	public event System.Action<Block>	OnBlockPressed;

	// gets called automatically if collider is attached
	void	OnMouseDown()
	{
		if (OnBlockPressed != null)
			OnBlockPressed(this);
	}

}
