using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class rotateCamera : MonoBehaviour {

	public gManager other;

	private float L;
	private float N;

	void Start () {
		L = (float) other.getL();
		N = (float) other.getN ();
		transform.position = new Vector3 (-N, L * 9.00f, -N);
		transform.LookAt(new Vector3(N,L/2.00f,N));
	}

	void Update () {
		if (Input.GetKey ("r")) {
			transform.RotateAround(new Vector3(N,L/2.00f,N), Vector3.up, 60*Time.deltaTime);
		}
	}
}
