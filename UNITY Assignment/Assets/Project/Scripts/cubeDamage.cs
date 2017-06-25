using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class cubeDamage : MonoBehaviour {

	private int damage;

	// Use this for initialization
	void Start () {
		damage = 3;
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	public void hitCube(){
		damage--;
	}

	public int getDamage(){
		return damage;
	}
}
