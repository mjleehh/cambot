import React from 'react'
import {useSelector, useDispatch} from 'react-redux'
import {State} from '../reducer'
import {Angles, Position, Rotations} from '../types'
import {Button, Table} from 'antd'
import {requestArmState} from '../actions'

const style: React.CSSProperties = {
    margin: '1em',
}

const PositionDisplay: React.FunctionComponent = () => {
    const tool = useSelector<State, Position>(({armState: {toolPosition}}) => toolPosition)
    const wrist = useSelector<State, Position>(({armState: {wristPosition}}) => wristPosition)
    const angles = useSelector<State, Angles>(({armState: {inverse}}) => inverse)
    const rotations = useSelector<State, Rotations>(({armState: {rotations}}) => rotations)
    const dispatch = useDispatch()
    return <div style={style}>
        Tool:
        <div>x: {tool.x}</div>
        <div>y: {tool.y}</div>
        <div>z: {tool.z}</div>
        Wrist:
        <div>x: {wrist.x}</div>
        <div>y: {wrist.y}</div>
        <div>z: {wrist.z}</div>
        Angles:
        <div>theta: {angles.theta / Math.PI * 180} | {rotations.yaw.deg} </div>
        <div>phi0: {angles.phi0 / Math.PI * 180} | {rotations.lowerVert.deg}</div>
        <div>phi1: {angles.phi1 / Math.PI * 180} | {rotations.upperVert.deg}</div>
        <Button onClick={() => dispatch(requestArmState())}>get state!</Button>
    </div>
}
PositionDisplay.displayName = 'PositionDisplay'
export default PositionDisplay