using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class FlockBehaviour : ScriptableObject
{
    // FlockAgent agent - calculation of movement of current agent
    // List<Transform> context - what neighbours are aroung agent in list
    public abstract Vector2 CalculateMove(FlockAgent agent, List<Transform> context, Flock flock);
}

