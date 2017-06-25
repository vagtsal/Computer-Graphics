using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
//using UnityEditor;

//using System.IO;

public class gManager : MonoBehaviour {

	public TextAsset mazeTXT;

	public GameObject redCube;
	public GameObject greenCube;
	public GameObject blueCube;
	public GameObject T1Cube;
	public GameObject T2Cube;
	public GameObject T3Cube;
	public GameObject colliderCube;

	public GameObject player;
	public GameObject camera3rd;
	public GameObject dCamera;
	public GameObject cylinder;
	public hit hitScript;
	public Text textScore;
	public Text textHUD;
	public GameObject gameoverCanvas;

	private int L;
	private int N;
	private int K;

	private bool flagMode1st;
	private bool flagTop;
	private bool gameoverFlag;
	private bool launchFlag;

	private float scale;


	void Awake () {
		flagTop = false;
		gameoverFlag = false;
		launchFlag = true;

		string[] lines = mazeTXT.text.Split("\n"[0]);

		L = int.Parse(lines[0].Split('=')[1]);
		N = int.Parse(lines[1].Split('=')[1]);
		K = int.Parse(lines[2].Split('=')[1]);

		dCamera.transform.position = new Vector3 (dCamera.transform.position.x, dCamera.transform.position.y + L * scale, dCamera.transform.position.z);

		scale = redCube.transform.localScale[0];
		int level;
		bool playerFlag = true;
		for (int i=0; i<L; i++){
			level = 4 + i * (N + 1);
			for (int j=0; j<N; j++){
				for (int k=0; k<N; k++){
					switch (lines [level + j].Split (new char[0], System.StringSplitOptions.RemoveEmptyEntries) [k]) {
					case "R":
						Instantiate (redCube, new Vector3 (j*scale, i*scale, k*scale), Quaternion.identity);
						break;
					case "G":
						Instantiate (greenCube, new Vector3 (j*scale, i*scale, k*scale), Quaternion.identity);
						break;
					case "B":
						Instantiate (blueCube, new Vector3 (j*scale, i*scale, k*scale), Quaternion.identity);
						break;
					case "T1":
						Instantiate (T1Cube, new Vector3 (j*scale, i*scale, k*scale), Quaternion.identity);
						break;
					case "T2":
						Instantiate (T2Cube, new Vector3 (j*scale, i*scale, k*scale), Quaternion.identity);
						break;
					case "T3":
						Instantiate (T3Cube, new Vector3 (j*scale, i*scale, k*scale), Quaternion.identity);
						break;
					case "E":
						if (i == 0) {
							if (playerFlag) {
								player.transform.position = new Vector3 (j * scale, i, k * scale);
								player.transform.rotation = Quaternion.identity;
								cylinder.transform.position = player.transform.position;
								player.SetActive (false);
								//myPlayer = Instantiate (player, new Vector3 (j * scale, i, k * scale), Quaternion.identity);
								//myCylinder = Instantiate (cylinder,  new Vector3 (j * scale, i, k * scale), Quaternion.identity);
								playerFlag = false;
							}
							//Instantiate (colliderCube, new Vector3 (j * scale, -scale, k * scale), Quaternion.identity);
						}
						/*if (i == L-1) {
							Instantiate (colliderCube, new Vector3 (j * scale, L*scale, k * scale), Quaternion.identity);
						}
						if (j == 0) {
							Instantiate (colliderCube, new Vector3 (-scale, i * scale, k * scale), Quaternion.identity);
						}
						if (j == N-1) {
							Instantiate (colliderCube, new Vector3 (N*scale, i * scale, k * scale), Quaternion.identity);
						}
						if (k == 0) {
							Instantiate (colliderCube, new Vector3 (j*scale, i * scale, -scale), Quaternion.identity);
						}
						if (k == N-1) {
							Instantiate (colliderCube, new Vector3 (j*scale, i * scale, N*scale), Quaternion.identity);
						}*/
						break;
					}
				}
			}
		}
		for (int i=-1; i<L+1; i++){
			for (int j=-1; j<N+1; j++){
				if (i == -1) {
					for (int k = -1; k < N + 1; k++) {
						Instantiate (colliderCube, new Vector3 (j * scale, i * scale, k * scale), Quaternion.identity);
					}
				}
				else {
					if (i == L) {
						if (j == -1 || j == N) {
							for (int k = -1; k < N + 1; k++) {
								Instantiate (colliderCube, new Vector3 (j * scale, i * scale, k * scale), Quaternion.identity);
								Instantiate (colliderCube, new Vector3 (j * scale, (i+1) * scale, k * scale), Quaternion.identity);
							}
						} 
						else {
							Instantiate (colliderCube, new Vector3 (j * scale, i * scale, -scale), Quaternion.identity);
							Instantiate (colliderCube, new Vector3 (j * scale, i * scale, N * scale), Quaternion.identity);
							Instantiate (colliderCube, new Vector3 (j * scale, (i+1) * scale, -scale), Quaternion.identity);
							Instantiate (colliderCube, new Vector3 (j * scale, (i+1)  * scale, N * scale), Quaternion.identity);
						}
					}
					else {
						if (j == -1 || j == N) {
							for (int k = -1; k < N + 1; k++) {
								Instantiate (colliderCube, new Vector3 (j * scale, i * scale, k * scale), Quaternion.identity);
							}
						} 
						else {
							Instantiate (colliderCube, new Vector3 (j * scale, i * scale, -scale), Quaternion.identity);
							Instantiate (colliderCube, new Vector3 (j * scale, i * scale, N * scale), Quaternion.identity);
						}
					}
				}
			}
		}
	}

	void Update () {
		if (gameoverFlag == false) {
			if (Input.GetKeyDown ("v")) {
				if (launchFlag) {
					launchFlag = false;
				}
				if (flagMode1st) {
					player.SetActive (false);
					cylinder.transform.position = player.transform.position;
					cylinder.SetActive (true);
					camera3rd.SetActive (true);
					flagMode1st = false;
				} 
				else {
					cylinder.SetActive (false);
					player.SetActive (true);
					camera3rd.SetActive (false);
					flagMode1st = true;
				}
			}

			if (player.transform.position.y > L * scale - 0.3) {
				flagTop = true;
				if (Input.GetKeyDown ("e")) {
					gameover ((N * N - (int)Time.time - (hitScript.getHamUsed ()) * 50));
				}
			} 
			else {
				flagTop = false;
			}

			if (launchFlag) {
				textHUD.text = "Hammers: " + K;
			} 
			else {
				textHUD.text = "Hammers: " + hitScript.getHammers ();
			}


			if (N * N - (int)Time.time - (hitScript.getHamUsed ()) * 50 > 0) {
				if (!flagTop) {
					textScore.text = "Potential Score: " + (N * N - (int)Time.time - (hitScript.getHamUsed ()) * 50);
				} 
				else {
					textScore.text = "Score: " + (N * N - (int)Time.time - (hitScript.getHamUsed ()) * 50);
				}
			} 
			else {
				gameover (0);
			}
		}
		if (Input.GetKeyDown ("x")) {
			Application.Quit ();
		}
	}

	private void gameover(int score){
		
		gameoverCanvas.GetComponent<Canvas> ().enabled = true;
		gameoverCanvas.GetComponentInChildren<Text>().text = "Final Score: " + score;
		camera3rd.SetActive (true);
		player.SetActive (false);
		gameoverFlag = true;
	}


	public int getL(){
		return L;
	}
							
	public int getN(){
		return N;
	}

	public int getK(){
		return K;
	}

}
