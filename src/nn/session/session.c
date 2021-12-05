// session.c

#include "session.h"

void _nn_train(struct nn_Session *session, nn_Model *model) {
    // // TODO: make this an argument, in session or directly here
    size_t verb_update_step = 0;

    nn_InOutTuple *tuple_array =
        session->dataset->train.data_collection.iot_array;
    size_t sample_size = session->dataset->train.data_collection.num_tuples;

    bool loss_threshold_condition = true;

    verbose(" - Training for %lu epochs -", session->nb_epochs);

    // incrementing directly epoch : so the verbose does not have to add 1
    // when printing (first epoch : 1, not 0)
    for (size_t epoch = 0;
         epoch++ < session->nb_epochs && loss_threshold_condition;) {

        if (session->verb_mode) {
            verbose("Epoch number: %ld", epoch);
        }

        ProgressBar training_bar = createProgressBar("Training", 0, sample_size,
                                                     DEFAULT_PROGRESSBAR_WIDTH);

        shuffleArray(tuple_array, sample_size);
        size_t i = 0;
        double loss_buffer = 0;
        while (i < sample_size) {
            // update the progress bar if necessary
            if (verb_update_step-- == 0) {
                updateProgressBar(&training_bar, i);
                verb_update_step = VERB_UPDATE_STEP;
            }

            // feed forward the input at index 'i'
            _nn_feedForward(model, tuple_array[i].input.values);

            // we calculate the loss function
            double error =
                applyLosses(model->layers[model->num_layers - 1],
                            tuple_array[i].output.values, model->loss);

            loss_buffer += error;

            _nn_backPropagation(model, tuple_array[i].output.values);
            _nn_updateWeights(model, session->learning_rate);

            i++;

            // if (session->verb_mode && i % nb_verb_step == 0) {
            // 	verbose(" train session run: %.2f%c  done (loss: %lf)", 100.0 *
            // (double)i/sample_size, '%', error);
            // }
        }

        endProgressBar(&training_bar);

        loss_buffer /= sample_size;
        // we continue as long as we do not reached loss threshold
        // or we continue as long as we have epochs to do
        loss_threshold_condition = !session->stop_on_loss_threshold_reached ||
                                   loss_buffer > session->loss_threshold;

        verbose("Epoch finished with:\n - avg loss: %lf\n", loss_buffer);
    }
    return;
}

void _nn_train_one_hot(struct nn_Session *session, nn_Model *model) {
    // // TODO: make this an argument, in session or directly here
    size_t verb_update_step = 0;

    nn_InOutTuple *tuple_array =
        session->dataset->train.data_collection.iot_array;
    size_t sample_size = session->dataset->train.data_collection.num_tuples;

    bool loss_threshold_condition = true;

    verbose(" * Training for %lu epochs on %lu samples *", session->nb_epochs,
            sample_size);

    Logger loss_logger = createLogger(session->loss_log_file);
    Logger right_logger = createLogger(session->right_log_file);

    // incrementing directly epoch : so the verbose does not have to add 1
    // when printing (first epoch : 1, not 0)
    for (size_t epoch = 0;
         epoch++ < session->nb_epochs && loss_threshold_condition;) {
        if (session->verb_mode)
            verbose(" * Doing epoch %lu/%lu", epoch, session->nb_epochs);

        ProgressBar training_bar = createProgressBar("Training", 0, sample_size,
                                                     DEFAULT_PROGRESSBAR_WIDTH);

        shuffleArray(tuple_array, sample_size);
        double loss_buffer = 0;
        int num_right_predictions = 0;
        for (size_t i = 0; i < sample_size; i++) {
            // update the progress bar if necessary
            if (verb_update_step-- == 0) {
                updateProgressBar(&training_bar, i);
                if (i != 0) {
                    updateLogger(&loss_logger, loss_buffer / (double)i);
                    updateLogger(&right_logger,
                                 (double)num_right_predictions / (double)i);
                }
                verb_update_step = VERB_UPDATE_STEP;
            }

            // feed forward the input at index 'i'
            _nn_feedForward(model, tuple_array[i].input.values);

            // we calculate the loss
            double error =
                applyLosses(model->layers[model->num_layers - 1],
                            tuple_array[i].output.values, model->loss);

            loss_buffer += error;

            nn_Layer *output_layer = model->layers[model->num_layers - 1];

            // get maximised value by the model
            size_t max_index = 0;
            for (size_t j = 1; j < output_layer->num_nodes; j++)
                if (output_layer->nodes[j]->value >
                    output_layer->nodes[max_index]->value)
                    max_index = j;
            // get index of hot value (1.0) in output values
            size_t result_index = 0;
            for (; result_index < tuple_array[i].output.num_values;
                 result_index++)
                if (tuple_array[i].output.values[result_index]) // != 0
                    break;
            // indices must be the same for the model to have rightly predicted
            if (max_index == result_index)
                num_right_predictions++;

            _nn_backPropagation(model, tuple_array[i].output.values);
            _nn_updateWeights(model, session->learning_rate);
        }

        endProgressBar(&training_bar);

        loss_buffer /= sample_size;
        // we continue as long as we do not reached loss threshold
        // or we continue as long as we have epochs to do
        loss_threshold_condition = !session->stop_on_loss_threshold_reached ||
                                   loss_buffer > session->loss_threshold;

        if (session->verb_mode)
            verbose("Epoch finished with:\n - avg loss: %lf\n - avg right: "
                    "%.2f %c\n",
                    loss_buffer,
                    100.0 * (double)num_right_predictions / sample_size, '%');
    }

    endLogger(&loss_logger);
    endLogger(&right_logger);

    return;
}

void _nn_test(struct nn_Session *session, nn_Model *model) {
    nn_InOutTuple *tuple_array =
        session->dataset->test.data_collection.iot_array;
    size_t sample_size = session->dataset->test.data_collection.num_tuples;

    shuffleArray(tuple_array, sample_size);
    for (size_t i = 0; i < sample_size; i++) {
        verbose("Testing Tuple:");
        tuple_array[i].printTuple(tuple_array[i]);
        _nn_feedForward(model, tuple_array[i].input.values);

        verbose("Result:");
        for (size_t j = 0; j < model->layers[model->num_layers - 1]->num_nodes;
             j++)
            verbose_no_endline(
                "%lf ", model->layers[model->num_layers - 1]->nodes[j]->value);
        verbose_endline();
        // we calculate the loss function
        double error = applyLosses(model->layers[model->num_layers - 1],
                                   tuple_array[i].output.values, model->loss);
        verbose("Losses error = %f", error);
    }
}

void _nn_test_one_hot(struct nn_Session *session, nn_Model *model) {
    // TODO: add an average right prediction for each individual
    // value of the one-hot values
    // verbose("Testing the model as one-hot");
    size_t num_steps_verb = 0;

    nn_InOutTuple *tuple_array =
        session->dataset->test.data_collection.iot_array;
    size_t num_samples = session->dataset->test.data_collection.num_tuples;
    double loss_sum = 0;
    int num_right_predictions = 0;
    nn_Layer *output_layer = model->layers[model->num_layers - 1];
    int *num_right_predictions_specific =
        mem_calloc(output_layer->num_nodes, sizeof(int));
    int *num_total_predictions_specific =
        mem_calloc(output_layer->num_nodes, sizeof(int));

    ProgressBar testing_bar = createProgressBar(
        "Testing (one-hot)", 0, num_samples, DEFAULT_PROGRESSBAR_WIDTH);

    for (size_t i = 0; i < num_samples; i++) {
        // verbosity
        if (session->verb_mode) {
            if (num_steps_verb-- == 0) {
                updateProgressBar(&testing_bar, i);
                num_steps_verb = VERB_UPDATE_STEP;
            }
        }
        // model prediction
        double *output_values =
            _nn_useModel(model, tuple_array[i].input.values);
        // get maximised value by the model
        size_t max_index = 0;
        for (size_t j = 1; j < output_layer->num_nodes; j++)
            if (output_values[j] > output_values[max_index])
                max_index = j;
        mem_free(output_values);
        // get index of hot value (1.0) in output values
        size_t result_index = 0;
        for (; result_index < tuple_array[i].output.num_values; result_index++)
            if (tuple_array[i].output.values[result_index]) // != 0
                break;
        // indices must be the same for the model to have rightly predicted
        if (max_index == result_index) {
            num_right_predictions++;
            num_right_predictions_specific[result_index]++;
        }
        num_total_predictions_specific[result_index]++;
        // calculate the loss
        double error = applyLosses(model->layers[model->num_layers - 1],
                                   tuple_array[i].output.values, model->loss);
        // add the loss to a total (for later average loss calculation)
        loss_sum += error;
    }
    endProgressBar(&testing_bar);
    // print averages
    double avg_loss = (double)loss_sum / (double)num_samples;
    double avg_right_predictions =
        (double)num_right_predictions / (double)num_samples;
    // print averages
    verbose("Testing finished with:");
    verbose(" - avg loss: %lf", avg_loss);
    verbose(" - avg right: %.2f %c", 100.0 * avg_right_predictions, '%');
    verbose("Specific right predictions:");
    for (size_t i = 0; i < output_layer->num_nodes; i++) {
        int nrs = num_right_predictions_specific[i];
        int nts = num_total_predictions_specific[i];
        double avg_right_specific = (double)nrs / (double)nts;
        verbose(" * %lu : %.2f%c", i + 1, 100.0 * avg_right_specific, '%');
    }
}

nn_Session *nn_createTestSession(nn_DataSet *dataset, bool verbose) {
    return nn_createSession(dataset, 0, 0.0, false, verbose, 0.0, NULL, NULL);
}

nn_Session *nn_createSession(nn_DataSet *dataset, unsigned int nb_epochs,
                             double loss_threshold,
                             bool stop_on_loss_threshold_reached, bool verbose,
                             double learning_rate, const char *loss_log_file,
                             const char *right_log_file) {
    nn_Session *session = mem_malloc(sizeof(nn_Session));

    session->dataset = dataset;
    session->nb_epochs = nb_epochs;
    session->loss_threshold = loss_threshold;
    session->verb_mode = verbose;
    session->learning_rate = learning_rate;

    session->stop_on_loss_threshold_reached = stop_on_loss_threshold_reached;

    session->loss_log_file = loss_log_file;
    session->right_log_file = right_log_file;

    session->train = &_nn_train;
    session->train_one_hot = &_nn_train_one_hot;
    session->test = &_nn_test;
    session->test_one_hot = &_nn_test_one_hot;

    return session;
}

void nn_freeSession(nn_Session *session) {
    _nn_freeDataSet(*session->dataset);
    mem_free(session);
}
