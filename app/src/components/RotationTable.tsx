import React from 'react'
import {useSelector} from 'react-redux'
import {State} from '../reducer'
import {Rotations} from '../types'
import _ from 'lodash'
import {Table} from 'antd'

const style: React.CSSProperties = {
    margin: '1em',
}

const columns = [
    {title: 'actuator', dataIndex: 'actuator', key: 'actuator'},
    {title: 'rotation (deg)', dataIndex: 'deg', key: 'rotation'},
    {title: 'rotation (steps) ', dataIndex: 'steps', key: 'steps'},
]

function convertFullRotationsToTable(rotations: Rotations) {
    return _.map(rotations, ({deg, steps}, actuator) => ({key: actuator, actuator, deg, steps}))
}

const RotationTable: React.FunctionComponent = () => {
    const rotations = useSelector<State, Rotations>(({armState: {rotations}}) => rotations)

    const tableData = convertFullRotationsToTable(rotations)
    return <Table pagination={false} dataSource={tableData} columns={columns}/>
}
RotationTable.displayName = 'RotationTable'
export default RotationTable