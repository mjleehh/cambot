import {
    RequestYawAction,
    REQUEST_YAW,
    REQUEST_UPPER_PITCH,
    RequestUpperPitchAction,
    REQUEST_LOWER_PITCH, REQUEST_PITCH, REQUEST_ROLL, RequestRollAction, RequestPitchAction
} from './actions'
import {takeEvery, call, select} from 'redux-saga/effects'
import api from './api'

function* yawSaga({payload: direction}: RequestYawAction) {
    const {numSteps, speed} = yield select()
    yield call([api, api.yaw], direction, numSteps, speed)
}

function* lowerPitchSaga({payload: direction}: RequestUpperPitchAction) {
    const {numSteps, speed} = yield select()
    yield call([api, api.lowerPitch], direction, numSteps, speed)
}

function* upperPitchSaga({payload: direction}: RequestUpperPitchAction) {
    const {numSteps, speed} = yield select()
    yield call([api, api.upperPitch], direction, numSteps, speed)
}

function* rollSaga({payload: direction}: RequestRollAction) {
    const {numSteps, speed} = yield select()
    yield call([api, api.roll], direction, numSteps, speed)
}

function* pitchSaga({payload: direction}: RequestPitchAction) {
    const {numSteps, speed} = yield select()
    yield call([api, api.pitch], direction, numSteps, speed)
}

export default function* rotateRobotSaga() {
    yield takeEvery(REQUEST_YAW, yawSaga)
    yield takeEvery(REQUEST_UPPER_PITCH, upperPitchSaga)
    yield takeEvery(REQUEST_LOWER_PITCH, lowerPitchSaga)
    yield takeEvery(REQUEST_ROLL, rollSaga)
    yield takeEvery(REQUEST_PITCH, pitchSaga)
}