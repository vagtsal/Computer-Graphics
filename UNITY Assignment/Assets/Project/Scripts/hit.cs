using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class hit : MonoBehaviour {
	private cubeDamage cubeD;

	public gManager gameM;
	public GameObject smallCube;
	public Material hammerMat;
	public AudioSource hitSound;
	public AudioSource destroySound;
	public GameObject pickUpHammer;

	private int hammerDamage;
	private int hammers;
	private int HammersUsed;

	void Start(){
		hammerDamage = 100;
		hammers = gameM.getK ();

		hammerMat.color = new Color (1f, 0f, 0f, 1f);
	}


	void Update () {
		if (Input.GetButtonDown("Fire1")) {
			this.GetComponent<Animation>().Play("Hammer");
		}

		if (this.GetComponent<Animation> ().isPlaying == true){
			this.GetComponent<Collider> ().isTrigger = true;
		} else {
			this.GetComponent<Collider> ().isTrigger = false;
		}
	}

	void OnTriggerEnter(Collider other) {
		if (other.gameObject.CompareTag ("cube")) {
			hitSound.Play ();
			hammerDamage -= 10;
			hammerMat.color = new Color (hammerMat.color.r - 0.08f, 0f, 0f, 1f);
			if (hammerDamage == 0) {
				hammers--;
				hammerDamage = 100;
				HammersUsed++;
				hammerMat.color = new Color (1f, 0f, 0f, 1f);
				if (hammers == 0) {
					this.gameObject.SetActive (false);
				}
			}

			cubeD = other.gameObject.GetComponent<cubeDamage> ();
			cubeD.hitCube ();
			if (cubeD.getDamage () == 0) {
				destroySound.Play ();

				GameObject b1 = Instantiate (smallCube, new Vector3 (other.gameObject.transform.position.x - 0.5f, other.gameObject.transform.position.y + 0.5f, other.gameObject.transform.position.z + 0.5f), Quaternion.identity);
				GameObject b2 = Instantiate (smallCube, new Vector3 (other.gameObject.transform.position.x - 0.5f, other.gameObject.transform.position.y + 0.5f, other.gameObject.transform.position.z - 0.5f), Quaternion.identity);
				GameObject b3 = Instantiate (smallCube, new Vector3 (other.gameObject.transform.position.x + 0.5f, other.gameObject.transform.position.y + 0.5f, other.gameObject.transform.position.z + 0.5f), Quaternion.identity);
				GameObject b4 = Instantiate (smallCube, new Vector3 (other.gameObject.transform.position.x - 0.5f, other.gameObject.transform.position.y + 0.5f, other.gameObject.transform.position.z - 0.5f), Quaternion.identity);
				GameObject b5 = Instantiate (smallCube, new Vector3 (other.gameObject.transform.position.x - 0.75f, other.gameObject.transform.position.y - 0.5f, other.gameObject.transform.position.z + 0.75f), Quaternion.identity);
				GameObject b6 = Instantiate (smallCube, new Vector3 (other.gameObject.transform.position.x - 0.75f, other.gameObject.transform.position.y - 0.5f, other.gameObject.transform.position.z - 0.75f), Quaternion.identity);
				GameObject b7 = Instantiate (smallCube, new Vector3 (other.gameObject.transform.position.x + 0.75f, other.gameObject.transform.position.y - 0.5f, other.gameObject.transform.position.z + 0.75f), Quaternion.identity);
				GameObject b8 = Instantiate (smallCube, new Vector3 (other.gameObject.transform.position.x - 0.75f, other.gameObject.transform.position.y - 0.5f, other.gameObject.transform.position.z - 0.75f), Quaternion.identity);

				b1.GetComponent<Renderer>().material = other.gameObject.GetComponent<Renderer>().material;
				b2.GetComponent<Renderer>().material = other.gameObject.GetComponent<Renderer>().material;
				b3.GetComponent<Renderer>().material = other.gameObject.GetComponent<Renderer>().material;
				b4.GetComponent<Renderer>().material = other.gameObject.GetComponent<Renderer>().material;
				b5.GetComponent<Renderer>().material = other.gameObject.GetComponent<Renderer>().material;
				b6.GetComponent<Renderer>().material = other.gameObject.GetComponent<Renderer>().material;
				b7.GetComponent<Renderer>().material = other.gameObject.GetComponent<Renderer>().material;
				b8.GetComponent<Renderer>().material = other.gameObject.GetComponent<Renderer>().material;

				Destroy (b1, 0.85f);
				Destroy (b2, 0.85f);
				Destroy (b3, 0.85f);
				Destroy (b4, 0.85f);
				Destroy (b5, 0.85f);
				Destroy (b6, 0.85f);
				Destroy (b7, 0.85f);
				Destroy (b8, 0.85f);

				if (Random.value > 0.6) {
					Instantiate (pickUpHammer, other.gameObject.transform.position, Quaternion.identity).transform.Rotate (225f, 180f, 90f);
				}

				Destroy (other.gameObject);
			}
		}
	}

	public int getHamUsed(){
		return HammersUsed;
	}

	public void setHammers(){
		hammers++;
	}

	public int getHammers(){
		return hammers;
	}
}
