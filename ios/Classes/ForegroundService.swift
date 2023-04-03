import Flutter
import BackgroundTasks

@available(iOS 13.0, *)
class BackgroundService: NSObject {
    static let shared = BackgroundService()

    private override init() {}

    func start() {
        let taskScheduler = BGTaskScheduler.shared

        do {
            try taskScheduler.register(forTaskWithIdentifier: "my_background_task", using: nil) { task in
                // Perform your background task here
                task.setTaskCompleted(success: true)
            }
        } catch {
            print("Failed to register background task: \(error.localizedDescription)")
        }

        scheduleTask()
    }

    private func scheduleTask() {
        let request = BGProcessingTaskRequest(identifier: "my_background_task")
        request.requiresNetworkConnectivity = false
        request.requiresExternalPower = false

        do {
            try BGTaskScheduler.shared.submit(request)
        } catch {
            print("Failed to schedule background task: \(error.localizedDescription)")
        }
    }
}



