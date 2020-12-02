using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

/// <summary>
/// Contains the functions for whether the player wins or loses.
/// Will wait 3 seconds afterwards and reset the game to its
/// original state.
/// </summary>
public class winLose : MonoBehaviour
{
    private Text endText;
    // Start is called before the first frame update
    void Start()
    {
        endText = GameObject.FindGameObjectWithTag("winLose").GetComponent<Text>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    /// <summary>
	/// Displays "You lose!" once the player loses the game.
	/// </summary>
    public void loseGame()
    {
        endText.text = "You lose!";
        StartCoroutine(resetGame());
    }

    /// <summary>
	/// Displays "You win!" once the player wins the game.
	/// </summary>
    public void winGame()
    {
        endText.text = "You win!";
        StartCoroutine(resetGame());
    }

    /// <summary>
	/// Waits for 3 seconds, then loads the original scene of the game.
	/// </summary>
	/// <returns></returns>
    IEnumerator resetGame()
    {
        yield return new WaitForSeconds(3);
        SceneManager.LoadScene("Main");
    }
}
