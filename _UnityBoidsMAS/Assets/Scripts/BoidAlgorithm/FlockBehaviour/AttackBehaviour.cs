using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(menuName = "Flock/Behaviour/Attack")]
public class AttackBehaviour : FilteredFlockBehaviour // We are inheriting from FilteredFlockBehaviour behaviour
{
    public override Vector2 CalculateMove(FlockAgent agent, List<Transform> context, Flock flock)
    {
        // if no neighbours, no adjustment
        if (context.Count == 0)
            return Vector2.zero;

        // add all points together and average

        Vector2 attackMove = Vector2.zero;

        int nAvoid = 0;

        // add all neighbours
        // if we dont have filter than we'll just use context as it is; If we have filter then we apply that
        List<Transform> filteredContext = (filter == null) ? context : filter.Filter(agent, context);
        foreach (Transform item in filteredContext)
        {
            if ((Vector2.SqrMagnitude(flock.enemyLocalisation.position - agent.transform.position)) > flock.SquareAvoidanceRadius)
            {
                nAvoid++;
                attackMove += (Vector2)(flock.enemyLocalisation.position - agent.transform.position);
            }
        }

        if (nAvoid > 0)
            attackMove /= nAvoid;

        return attackMove;
    }
}
