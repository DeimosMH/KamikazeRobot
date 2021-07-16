using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(menuName = "Flock/Behaviour/Avoidance")]
public class AvoidanceBehaviour : FilteredFlockBehaviour // We are inheriting from flock behaviour
{
    public override Vector2 CalculateMove(FlockAgent agent, List<Transform> context, Flock flock)
    {
        // if no neighbours, no adjustment
        if (context.Count == 0) 
            return Vector2.zero; 

        // add all points together and average
        Vector2 avoidanceMove = Vector2.zero;

        int nAvoid = 0;

        // add all neighbours
        // if we dont have filter than we'll just use context as it is; If we have filter then we apply that
        List<Transform> filteredContext = (filter == null) ? context : filter.Filter(agent, context);
        foreach (Transform item in filteredContext)
        {
            if ((Vector2.SqrMagnitude(item.position - agent.transform.position)) < flock.SquareAvoidanceRadius)
            {
                nAvoid++;
                avoidanceMove += (Vector2)(agent.transform.position - item.position) * 2;
            }
            
        }
        
        if (nAvoid > 0) 
            avoidanceMove /= nAvoid; 

        return avoidanceMove;
    }
}

