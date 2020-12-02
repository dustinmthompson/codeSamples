using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Detonates shells, plays audio, and produces shell explosion
/// </summary>
public class shellMechanics : MonoBehaviour
{
    public ParticleSystem explosion;
    public AudioClip shellExplode;
    public AudioClip shellFire;
    private GameObject audioSource;

    // Start is called before the first frame update
    void Start()
    {
        audioSource = gameObject.transform.GetChild(1).gameObject;
        audioSource.GetComponent<AudioSource>().PlayOneShot(shellFire);
    }

    // Update is called once per frame
    void Update()
    {

    }

    private void OnCollisionEnter(Collision collision)
    {
        detonate();
    }

    /// <summary>
	/// Destroy shell, play shell audio, and produce explosion particles
	/// </summary>
    private void detonate()
    {
        // Separate particle system and play
        explosion.transform.parent = null;
        explosion.Play();

        // Separate audio source and play
        audioSource.transform.parent = null;
        audioSource.GetComponent<AudioSource>().PlayOneShot(shellExplode);

        // Destroy shell, audio source, and particle system
        Destroy(audioSource.gameObject, shellExplode.length);
        Destroy(explosion.gameObject, explosion.main.duration);
        Destroy(gameObject);
    }
}
