using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveToGoal : MonoBehaviour
{
    public Transform Goal;
    public float spaceBetween = 1.0f;

    void Update()
    {
        if (Vector3.Distance(Goal.position, transform.position) >= spaceBetween) 
        {
            Vector3 direction = Goal.position - transform.position; // What the direction AI have to move to reach the goal
            transform.Translate(direction * Time.deltaTime);        // deltaTime make move smooth, not instant
        }
    }
}
