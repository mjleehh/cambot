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
    GET_ROTATIONS,
    gotRotations,
    SET_HOME,
    MOVE_YAW
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

function* setHomeSaga(_: SetHomeAction) {
    yield call([api, api.setHome])
}

function* getRotationsSaga(_: SetHomeAction): any {
    const rotations = yield call([api, api.rotations])
    console.log(rotations.data)
}

function* pollPositionsSaga(): any{
    while (true) {
        try {
            const {data} = yield call([api, api.rotations])
            console.log(data)
            yield put(gotRotations(data))
            yield delay(1000)
        } catch (e) {
            console.log('polling positions failed')
        }
    }
}

function* moveYawSaga({payload: angle}: RequestRollAction) {
    yield call([api, api.moveYaw], angle, 1)
}

export default function* rootSaga() {
    yield takeEvery(REQUEST_YAW, yawSaga)
    yield takeEvery(REQUEST_UPPER_VERT, upperPitchSaga)
    yield takeEvery(REQUEST_LOWER_VERT, lowerPitchSaga)
    yield takeEvery(REQUEST_ROLL, rollSaga)
    yield takeEvery(REQUEST_PITCH, pitchSaga)
    yield takeEvery(SET_HOME, setHomeSaga)
    yield takeEvery(GET_ROTATIONS, getRotationsSaga)
    yield fork(pollPositionsSaga)
    yield takeEvery(MOVE_YAW, moveYawSaga)
}