using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

/// <summary>
/// Handles and displays the count of how many enemies
/// are left. At 0, the player will win the game.
/// </summary>
public class enemyCount : MonoBehaviour
{
    private GameObject enemyText;
    private GameObject canvas;
    private int enemies = 7;

    // Start is called before the first frame update
    void Start()
    {
        enemyText = GameObject.FindGameObjectWithTag("enemyCount");
        canvas = GameObject.FindGameObjectWithTag("canvas");
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    /// <summary>
	/// Decrements the enemy count every time an enemy is destroyed, then
	/// updates the display in the top right. Calls on winGame() if enemies
	/// has reached 0.
	/// </summary>
    public void decrement()
    {
        // Decrement enemy counter and display new
        enemies--;
        enemyText.GetComponent<Text>().text = enemies.ToString();

        // If no more enemies, win game
        if (enemies == 0)
        {
            canvas.GetComponent<winLose>().winGame();
        }
    }
}
