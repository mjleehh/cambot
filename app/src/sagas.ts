import {
    RequestYawAction,
    REQUEST_YAW,
    REQUEST_UPPER_VERT,
    RequestUpperVertAction,
    REQUEST_LOWER_VERT,
    REQUEST_PITCH,
    REQUEST_ROLL,
    RequestRollAction,
    RequestPitchAction,
    SetHomeAction,
    gotArmState,
    SET_HOME,
    MOVE_YAW, GO_HOME, MoveToAction, MOVE_TO
} from './actions'
import {takeEvery, call, select, fork, delay, put} from 'redux-saga/effects'
import api from './api'

function* yawSaga({payload: direction}: RequestYawAction) {
    const {numSteps, speed} = yield select()
    yield call([api, api.yaw], direction, numSteps, speed)
}

function* lowerPitchSaga({payload: direction}: RequestUpperVertAction) {
    const {numSteps, speed} = yield select()
    yield call([api, api.lowerVert], direction, numSteps, speed)
}

function* upperPitchSaga({payload: direction}: RequestUpperVertAction) {
    const {numSteps, speed} = yield select()
    yield call([api, api.upperVert], direction, numSteps, speed)
}

function* rollSaga({payload: direction}: RequestRollAction) {
    const {numSteps, speed} = yield select()
    yield call([api, api.roll], direction, numSteps, speed)
}

function* pitchSaga({payload: direction}: RequestPitchAction) {
    const {numSteps, speed} = yield select()
    yield call([api, api.pitch], direction, numSteps, speed)
}

function* setHomeSaga() {
    yield call([api, api.setHome])
}

function* goHomeSaga() {
    yield call([api, api.goHome])
}

function* pollArmStateSaga(): any{
    while (true) {
        try {
            const armState = yield call([api, api.armState])
            console.log(armState)
            yield put(gotArmState(armState))
        } catch (e) {
            console.log('polling positions failed')
        }
        yield delay(1000)
    }
}

function* moveYawSaga({payload: angle}: RequestRollAction) {
    yield call([api, api.moveYaw], angle, 1)
}

function* moveToSaga({payload: {x, y, z, speed}}: MoveToAction) {
    yield call([api, api.moveTo], x, y, z, speed)
}

export default function* rootSaga() {
    yield takeEvery(REQUEST_YAW, yawSaga)
    yield takeEvery(REQUEST_UPPER_VERT, upperPitchSaga)
    yield takeEvery(REQUEST_LOWER_VERT, lowerPitchSaga)
    yield takeEvery(REQUEST_ROLL, rollSaga)
    yield takeEvery(REQUEST_PITCH, pitchSaga)
    yield takeEvery(SET_HOME, setHomeSaga)
    yield takeEvery(GO_HOME, goHomeSaga)
    yield fork(pollArmStateSaga)
    yield takeEvery(MOVE_YAW, moveYawSaga)
    yield takeEvery(MOVE_TO, moveToSaga)
}