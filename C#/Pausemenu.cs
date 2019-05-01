using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Pausemenu : MonoBehaviour {

    public static bool GameIsPaused = false;
    // Use this for initialization
    public GameObject PauseMenuUI;
	// Update is called once per frame
    void Start()
    {

        PauseMenuUI.SetActive(false);
    }
	void Update () {
		if (Input.GetKeyDown(KeyCode.X))
        {
            if (GameIsPaused)
            {
                Resume(); 
            }
            else
            {
                Pause(); 
            }
        }
	}
    public void Resume()
    {
        PauseMenuUI.SetActive(false);
        Time.timeScale = 1f;
        GameIsPaused = false;
        Cursor.lockState = CursorLockMode.Locked;

    }
    void Pause()
    {
        PauseMenuUI.SetActive(true); 
        Time.timeScale = 0f;
        GameIsPaused = true;
        Cursor.lockState = CursorLockMode.None;
    }
}
