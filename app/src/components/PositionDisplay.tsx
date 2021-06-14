import React from 'react'
import {useSelector} from 'react-redux'
import {State} from '../reducer'
import {Position, Rotations} from '../types'
import _ from 'lodash'
import {Table} from 'antd'

const style: React.CSSProperties = {
    margin: '1em',
}

const PositionDisplay: React.FunctionComponent = () => {
    const {x, y, z} = useSelector<State, Position>(({armState: {position}}) => position)
    return <div style={style}>
        <div>{x}</div>
        <div>{y}</div>
        <div>{z}</div>
    </div>
}
PositionDisplay.displayName = 'PositionDisplay'
export default PositionDisplay