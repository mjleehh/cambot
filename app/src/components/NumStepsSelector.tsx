import {STEPS_1000_KEY, STEPS_100_KEY, STEPS_10_KEY, STEPS_1_KEY} from '../key-bindings'
import {Radio} from 'antd'
import {setNumSteps} from '../actions'
import React from 'react'
import {useDispatch, useSelector} from 'react-redux'
import {State} from '../reducer'

const style: React.CSSProperties = {
    margin: '1em',
}

const numStepsOptions = [
    {label: `1 step (${STEPS_1_KEY})`, value: 1},
    {label: `10 steps (${STEPS_10_KEY})`, value: 10},
    {label: `100 steps (${STEPS_100_KEY})`, value: 100},
    {label: `1000 steps (${STEPS_1000_KEY})`, value: 1000},
]

const NumStepsSelector: React.FunctionComponent = () => {
    const numSteps = useSelector<State, number>(({numSteps}) => numSteps)
    const dispatch = useDispatch()

    return <div style={style}>
        <Radio.Group optionType='button' options={numStepsOptions} value={numSteps} onChange={e => dispatch(setNumSteps(e.target.value))}/>
    </div>
}
NumStepsSelector.displayName = 'NumStepsSelector'
export default NumStepsSelector