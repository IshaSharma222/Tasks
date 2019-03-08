#include "gazebobehavior.h"

GazeboBehavior::
GazeboBehavior( const std::string teamName,
                int uNum,
                const map<string, string>& namedParams_,
                const string& rsg_)
    : NaoBehavior( teamName,
                   uNum,
                   namedParams_,
                   rsg_) {
}

void GazeboBehavior::
beam( double& beamX, double& beamY, double& beamAngle ) {
    beamX = -HALF_FIELD_X+.5;
    beamY = 0;
    beamAngle = 0;
}

SkillType GazeboBehavior::
selectSkill() {
    // Walk to the ball
    return goToTarget(ball);
    double distance, angle;
    getTargetDistanceAndAngle(ball, distance, angle);
    if (abs(angle) > 10) {
      return goToTargetRelative(VecPosition(), angle);
    } else {
      return SKILL_STAND;
    }
     // Walk to ball while always facing forward
    return goToTargetRelative(worldModel->g2l(ball), -worldModel->getMyAngDeg());

    // Dribble ball toward opponent's goal
    return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));

    // Kick ball toward opponent's goal
    return kickBall(KICK_FORWARD, VecPosition(HALF_FIELD_X, 0, 0)); // Basic kick
    return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0)); // IK kick
    //*Computes an estimated cost of reaching the appropriate position to execute a kick
    return computeKickCost(VecPosition Target, KICK_FORWARD);
    return computeKickCost(VecPosition Target, KICK_IK);
    //return SKILL_STAND;
}
