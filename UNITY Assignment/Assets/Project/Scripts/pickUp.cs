using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class pickUp : MonoBehaviour {
	private AudioSource pickupSound;
	private hit hitScript;

	void OnTriggerEnter (Collider other) {
		if (other.gameObject.CompareTag ("Player")) {
			pickupSound = GameObject.Find ("pickupSound").GetComponent<AudioSource> ();
			pickupSound.Play ();
			//this.transform.parent.gameObject.SetActive (false);
			hitScript = other.gameObject.GetComponentInChildren<hit>();
			hitScript.setHammers ();
			Destroy (this.transform.parent.gameObject);
		}
	}
}
