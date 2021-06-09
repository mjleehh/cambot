import {
    RequestYawAction,
    REQUEST_YAW,
    REQUEST_UPPER_PITCH,
    RequestUpperPitchAction,
    REQUEST_LOWER_PITCH, REQUEST_PITCH, REQUEST_ROLL, RequestRollAction, RequestPitchAction
} from './actions'
import {takeEvery, call} from 'redux-saga/effects'
import api from './api'

function* yawSaga({payload: {direction, steps, speed}}: RequestYawAction) {
    yield call([api, api.yaw], direction, steps, speed)
}

function* lowerPitchSaga({payload: {direction, steps, speed}}: RequestUpperPitchAction) {
    yield call([api, api.lowerPitch], direction, steps, speed)
}

function* upperPitchSaga({payload: {direction, steps, speed}}: RequestUpperPitchAction) {
    yield call([api, api.upperPitch], direction, steps, speed)
}

function* rollSaga({payload: {direction, steps, speed}}: RequestRollAction) {
    yield call([api, api.roll], direction, steps, speed)
}

function* pitchSaga({payload: {direction, steps, speed}}: RequestPitchAction) {
    yield call([api, api.pitch], direction, steps, speed)
}

export default function* rotateRobotSaga() {
    yield takeEvery(REQUEST_YAW, yawSaga)
    yield takeEvery(REQUEST_UPPER_PITCH, upperPitchSaga)
    yield takeEvery(REQUEST_LOWER_PITCH, lowerPitchSaga)
    yield takeEvery(REQUEST_ROLL, rollSaga)
    yield takeEvery(REQUEST_PITCH, pitchSaga)
}