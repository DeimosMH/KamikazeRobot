using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(menuName = "Flock/Behaviour/Alignment")]
public class AlignmentBehaviour : FilteredFlockBehaviour // We are inheriting from flock behaviour
{
    public override Vector2 CalculateMove(FlockAgent agent, List<Transform> context, Flock flock)
    {
        // if no neighbours, maintain current alignment/heading
        if (context.Count == 0)
            return agent.transform.up;

        // add all points together and average
        Vector2 alignmentMove = Vector2.zero;

        // add all neighbours
            // if we dont have filter than we'll just use context as it is; If we have filter then we apply that
            List<Transform> filteredContext = (filter == null) ? context : filter.Filter(agent, context);
        foreach (Transform item in filteredContext)
        {
            alignmentMove += (Vector2)item.transform.up;
        }
        alignmentMove /= context.Count; // average distance

        return alignmentMove;
    }
}
