import React from 'react'
import LandingHero from './components/LandingHero'
import LiveDrops from './components/LiveDrops'
import FutureReleases from './components/FutureReleases'
import EndedDrops from './components/EndedDrops'
import authHOC from '../common/hoc/authHOC'
import styles from './index.module.less'

const Launchpad2 = () => {
    return (
        <div className={styles.page}>
            <LandingHero />
            <main className={styles.container}>
                <LiveDrops />
                <FutureReleases />
                <EndedDrops />
            </main>
        </div>
    )
}

export default authHOC(true)(Launchpad2)
