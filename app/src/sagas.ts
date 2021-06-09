import {RequestYawAction, REQUEST_YAW, REQUEST_UPPER_PITCH, RequestUpperPitchAction} from './actions'
import {takeEvery, call} from 'redux-saga/effects'
import api from './api'

function* yawSaga({payload: {direction, steps, speed}}: RequestYawAction) {
    yield call([api, api.yaw], direction, steps, speed)
}

function* upperPitchSaga({payload: {direction, steps, speed}}: RequestUpperPitchAction) {
    yield call([api, api.upperPitch], direction, steps, speed)
}

export default function* rotateRobotSaga() {
    yield takeEvery(REQUEST_YAW, yawSaga)
    yield takeEvery(REQUEST_UPPER_PITCH, upperPitchSaga)
}