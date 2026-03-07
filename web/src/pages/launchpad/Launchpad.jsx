import React, { useState, useEffect, useContext, useMemo, useCallback } from 'react'
import { Tabs, Pagination, Empty, Spin, App } from 'antd'
import dayjs from 'dayjs'
import Banner from '@/pages/launchpad/components/Banner/Banner'
import ProjectCard from '@/pages/launchpad/components/ProjectCard/ProjectCard'
import FinishedProjectCard from '@/pages/launchpad/components/FinishedProjectCard/FinishedProjectCard'
import authHOC from '../common/hoc/authHOC'
import styles from '@/pages/launchpad/launchpad.module.less'
import { fetchLaunchpadInfo } from '@/api/launchpad'
import RootContext from '../rootContext'

const Launchpad = () => {
  const { message } = App.useApp()
  const [activeTab, setActiveTab] = useState('xlaunch')
  const [currentPage, setCurrentPage] = useState(1)
  const pageSize = 6 // 每页显示6个项目
  const { userInfo } = useContext(RootContext)

  const [now, setNow] = useState(() => Date.now())

  const refreshNow = useCallback(() => {
    setNow(Date.now())
  }, [])

  const [launchpadList, setLaunchpadList] = useState([])
  const [loading, setLoading] = useState(false)
  // 根据 CloseTime 将接口数据分成进行中和已结束两部分
  const { activeList, finishedList } = useMemo(() => {
    const currentNow = now
    const active = []
    const finished = []

    launchpadList.forEach(item => {
      const closeTime = item?.CloseTime ? dayjs(item.CloseTime).valueOf() : null
      if (closeTime && currentNow >= closeTime) {
        finished.push(item)
      } else {
        active.push(item)
      }
    })

    return { activeList: active, finishedList: finished }
  }, [launchpadList, now])

  // 计算当前页显示的已结束项目
  const startIndex = (currentPage - 1) * pageSize
  const endIndex = startIndex + pageSize
  const currentFinishedProjects = finishedList.slice(startIndex, endIndex)
  const totalFinishedProjects = finishedList.length

  const getLaunchpadingData = () => {
    setLoading(true);
    fetchLaunchpadInfo({ page: 1, limit: 100 })
      .then((res) => {
        if (res.code === 0 && res?.data) {

          setLaunchpadList(res?.data?.launchpadList || []);
        }
      })
      .catch((err) => {
        message.error(err?.msg || '获取数据失败');
      })
      .finally(() => {
        setLoading(false);
      });

  };


  useEffect(() => {
    getLaunchpadingData();
  }, [])

  const tabItems = [
    {
      key: 'xlaunch',
      label: 'IFO 首发',
    },
    {
      key: 'giveaway',
      label: '碎片抽奖',
    },
    {
      key: 'trading',
      label: '交易赛',
    },
  ]

  return (
    <section className={styles.page}>
      <Spin spinning={loading} size="large">
        <div className={styles.container}>
          <Banner launchpadList={activeList} userInfo={userInfo} loading={loading} now={now} onCountdownEnd={refreshNow} />

          <div className={styles.tabsSection}>
            <Tabs
              activeKey={activeTab}
              onChange={setActiveTab}
              items={tabItems}
              className={styles.tabs}
            />

            <div className={styles.content}>
              {activeTab === 'xlaunch' && (
                <>
                  <div className={styles.activeProjectsSection}>
                    {activeList.length > 0 ? (
                      activeList.map(project => (
                        <ProjectCard key={project?.Id} project={project} now={now} onCountdownEnd={refreshNow} />
                      ))
                    ) : !loading && (
                      <div className={styles.emptyState}>
                        <p>暂无进行中的项目</p>
                      </div>
                    )}
                  </div>

                  <div className={styles.finishedSection}>
                    <h2 className={styles.sectionTitle}>已结束</h2>
                    {finishedList.length > 0 ? (
                      <>
                        <div className={styles.finishedProjectsGrid}>
                          {currentFinishedProjects.map(project => (
                            <FinishedProjectCard key={project?.Id} project={project} />
                          ))}
                        </div>

                        <div className={styles.paginationWrapper}>
                          <Pagination
                            current={currentPage}
                            total={totalFinishedProjects}
                            pageSize={pageSize}
                            onChange={setCurrentPage}
                            showSizeChanger={false}
                            className={styles.pagination}
                          />
                        </div>
                      </>
                    ) : !loading && (
                      <Empty description="暂无数据" style={{ paddingBottom: 60 }} />
                    )}
                  </div>
                </>
              )}

              {activeTab === 'giveaway' && (
                <div className={styles.emptyState}>
                  <p>暂无Giveaway活动</p>
                </div>
              )}

              {activeTab === 'trading' && (
                <div className={styles.emptyState}>
                  <p>暂无交易赛活动</p>
                </div>
              )}
            </div>
          </div>
        </div>
      </Spin>
    </section>
  )
}

export default authHOC(true)(Launchpad)
